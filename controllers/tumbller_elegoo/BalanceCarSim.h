/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-10-08 09:35:07
 * @LastEditTime: 2019-10-11 16:25:04
 * @LastEditors: Please set LastEditors
 */
#include <vector>
#include <KalmanFilter.cpp>


// From Arduino.h
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))


KalmanFilter kalmanfilter;

//Setting PID parameters

double kp_balance = 55, kd_balance = 0.75;
double kp_speed = 10, ki_speed = 0.26;
double kp_turn = 2.5, kd_turn = 0.5;

//Setting MPU6050 calibration parameters
double angle_zero = 0;            //x axle angle calibration
double angular_velocity_zero = 0; //x axle angular velocity calibration

volatile unsigned long encoder_count_right_a = 0;
volatile unsigned long encoder_count_left_a = 0;
float dt = 0.005, Q_angle = 0.001, Q_gyro = 0.005, R_angle = 0.5, C_0 = 1, K1 = 0.05;

int encoder_left_pulse_num_speed = 0;
int encoder_right_pulse_num_speed = 0;
double speed_control_output = 0;
double rotation_control_output = 0;
double speed_filter = 0;
int speed_control_period_count = 0;
double car_speed_integeral = 0;
double speed_filter_old = 0;
int setting_car_speed = 0;
int setting_turn_speed = 0;
float kalmanfilter_angle;
// char balance_angle_min = -27;
// char balance_angle_max = 27;
char balance_angle_min = -22;
char balance_angle_max = 22;

void carStop()
{
  //digitalWrite(AIN1, HIGH);
  //digitalWrite(BIN1, LOW);
  //digitalWrite(STBY_PIN, HIGH);
  //analogWrite(PWMA_LEFT, 0);
  //analogWrite(PWMB_RIGHT, 0);
}

void carForward(unsigned char speed)
{
  //digitalWrite(AIN1, 0);
  //digitalWrite(BIN1, 0);
  //analogWrite(PWMA_LEFT, speed);
  //analogWrite(PWMB_RIGHT, speed);
}

void carBack(unsigned char speed)
{
  //digitalWrite(AIN1, 1);
  //digitalWrite(BIN1, 1);
  //analogWrite(PWMA_LEFT, speed);
  //analogWrite(PWMB_RIGHT, speed);
}

std::vector<double> balanceCar(int16_t ax, int16_t ay, int16_t az, int16_t gx, int16_t gy, int16_t gz)
{
  double pwm_left = 0;
  double pwm_right = 0;
  encoder_left_pulse_num_speed += pwm_left < 0 ? -encoder_count_left_a : encoder_count_left_a;
  encoder_right_pulse_num_speed += pwm_right < 0 ? -encoder_count_right_a : encoder_count_right_a;
  encoder_count_left_a = 0;
  encoder_count_right_a = 0;
  kalmanfilter.Angle(ax, ay, az, gx, gy, gz, dt, Q_angle, Q_gyro, R_angle, C_0, K1);
  kalmanfilter_angle = kalmanfilter.angle;
  double balance_control_output = kp_balance * (kalmanfilter_angle - angle_zero) + kd_balance * (kalmanfilter.Gyro_x - angular_velocity_zero);

  speed_control_period_count++;
  if (speed_control_period_count >= 8)
  {
    speed_control_period_count = 0;
    double car_speed = (encoder_left_pulse_num_speed + encoder_right_pulse_num_speed) * 0.5;
    encoder_left_pulse_num_speed = 0;
    encoder_right_pulse_num_speed = 0;
    speed_filter = speed_filter_old * 0.7 + car_speed * 0.3;
    speed_filter_old = speed_filter;
    car_speed_integeral += speed_filter;
    car_speed_integeral += -setting_car_speed;
    car_speed_integeral = constrain(car_speed_integeral, -3000, 3000);
    speed_control_output = -kp_speed * speed_filter - ki_speed * car_speed_integeral;
    rotation_control_output = setting_turn_speed + kd_turn * kalmanfilter.Gyro_z;
  }

  pwm_left = balance_control_output - speed_control_output - rotation_control_output;
  pwm_right = balance_control_output - speed_control_output + rotation_control_output;

  pwm_left = constrain(pwm_left, -255, 255);
  pwm_right = constrain(pwm_right, -255, 255);
  if (kalmanfilter_angle < balance_angle_min || balance_angle_max < kalmanfilter_angle)
  {
    carStop();
  }

  if (pwm_left < 0)
  {
    //analogWrite(PWMA_LEFT, -pwm_left);
    pwm_left = -pwm_left;
  }
  if (pwm_right < 0)
  {
    //analogWrite(PWMB_RIGHT, -pwm_right);
    pwm_right = -pwm_right;
  }

  std::vector<double> data;
  data.assign(pwm_left, pwm_right);
  return data;
}

