// File:          tumbller_elegoo.cpp
// Date:
// Description:
// Author:
// Modifications:
//

#include <chrono>
#include <iostream>
#include <vector>

// You may need to add webots include files such as
// <webots/DistanceSensor.hpp>, <webots/Motor.hpp>, etc.
// and/or to add some other includes
#include <webots/Accelerometer.hpp>
#include <webots/Gyro.hpp>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>

#include <BalanceCarSim.h>

// All the webots classes are defined in the "webots" namespace
using namespace webots;

// Found specs
// https://robu.in/product/rs-37-500-rpm-high-torque-side-shaft-dc-geared-motor/#tab-specification
double torque_37GB520 = 0.196133; // N.m, from specs at 2 Kg-cm

// From Elegoo
unsigned long start_prev_time = 0;

uint64_t millis() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}


// This is the main program of your controller.
// It creates an instance of your Robot instance, launches its
// function(s) and destroys it at the end of the execution.
// Note that only one instance of Robot should be created in
// a controller program.
// The arguments of the main function can be specified by the
// "controllerArgs" field of the Robot node
int main(int argc, char **argv) {

  // create the Robot instance.
  Robot *robot = new Robot();

  // get the time step of the current world.
  // Balancing loop time from the Elegoo code (MSTimer2 period)
  int timeStep = 5; //(int)robot->getBasicTimeStep();

  // You should insert a getDevice-like function in order to get the
  // instance of a device of the robot. Something like:
  //  Motor *motor = robot->getMotor("motorname");
  //  DistanceSensor *ds = robot->getDistanceSensor("dsname");
  //  ds->enable(timeStep);
  Motor *leftMotor = robot->getMotor("left_motor");
  Motor *rightMotor = robot->getMotor("right_motor");
  //leftMotor->setPosition(INFINITY);
  //rightMotor->setPosition(INFINITY);
  //leftMotor->setVelocity(0.0);
  //rightMotor->setVelocity(0.0);
  
  Accelerometer *accelerometer = robot->getAccelerometer("MPU-6050-Accelerometer");
  accelerometer->enable(1);
  
  Gyro *gyroscope = robot->getGyro("MPU-6050-Gyroscope");
  gyroscope->enable(1);
  
  // Main loop:
  // - perform simulation steps until Webots is stopping the controller
  start_prev_time = millis();
  std::vector<double> motor_cmd;
  const double * acc;
  const double * gyr;
  while (robot->step(timeStep) != -1) {
    // Read the sensors:
    // Enter here functions to read sensor data, like:
    //  double val = ds->getValue();

    // Process sensor data here.

    // Enter here functions to send actuator commands, like:
    //  motor->setPosition(10.0);
    static unsigned long print_time;
    if (millis() - print_time > 100)
    {
      print_time = millis();
      //Serial.println(kalmanfilter.angle);
    }
    static unsigned long start_time;
    if (millis() - start_time < 10)
    {
      //carStop();
    }
    if (millis() - start_time == 2000) // Enter the pendulum, the car balances...
    {
      //key_value = '5';   
    }
    //leftMotor->setVelocity(3);
    //rightMotor->setVelocity(-3);
    acc = accelerometer->getValues();
    gyr = gyroscope->getValues();
    motor_cmd = balanceCar(acc[0], acc[1], acc[2], gyr[0], gyr[1], gyr[2]);
    leftMotor->setTorque(torque_37GB520 * motor_cmd.at(0));
    rightMotor->setTorque(torque_37GB520 * motor_cmd.at(1));
    
    std::cout << "Acc: " << acc[0] << " " << acc[1] << " ";
    std::cout << "Gyr: " << gyr[0] << " " << gyr[1] << " ";
    std::cout << "Cmd: ";
    for (double i: motor_cmd) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
  };

  // Enter here exit cleanup code.

  delete robot;
  return 0;
}
