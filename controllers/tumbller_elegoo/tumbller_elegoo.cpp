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
  std::vector<double> motors_pwm;
  const double * acc;
  const double * gyr;
  
  while (robot->step(timeStep) != -1) {
    acc = accelerometer->getValues();
    gyr = gyroscope->getValues();
    motors_pwm = balanceCar(acc[0], acc[1], acc[2], gyr[0], gyr[1], gyr[2]);

    double left = torque_37GB520 * motors_pwm.at(0) / 255.0;
    double right = torque_37GB520 * motors_pwm.at(1) / 255.0;
    leftMotor->setTorque(left);
    rightMotor->setTorque(right);

    //std::cout << "Acc: " << acc[0] << " " << acc[1] << " ";
    //std::cout << "Gyr: " << gyr[0] << " " << gyr[1] << " ";
    std::cout << "Cmd: ";
    for (double i: motors_pwm) {
      std::cout << i << " ";
    }
    std::cout << "Torque: " << left << " " << right << std::endl;
  };

  // Enter here exit cleanup code.

  delete robot;
  return 0;
}
