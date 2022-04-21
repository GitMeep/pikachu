#include <Arduino.h>
#line 28 "c:\\Users\\pelle\\OneDrive - TEC\\3g\\El\\Eksamensprojekt\\Kode\\Pikachu.ino"
void setup();
#line 99 "c:\\Users\\pelle\\OneDrive - TEC\\3g\\El\\Eksamensprojekt\\Kode\\Pikachu.ino"
void loop();
#line 0 "c:\\Users\\pelle\\OneDrive - TEC\\3g\\El\\Eksamensprojekt\\Kode\\Pikachu.ino"
#line 1 "c:\\Users\\pelle\\OneDrive - TEC\\3g\\El\\Eksamensprojekt\\Kode\\BalanceControl.cpp"
#include "BalanceControl.h"

float BalanceControl::_measuredAngle = 0;
PID BalanceControl::_pid(50, 10, 0);

void BalanceControl::init() {
  _pid.setLimits(-255, 255);
}

float BalanceControl::update(const float & angle, const unsigned int & dt) {
  _measuredAngle += 0.6 * (angle - _measuredAngle);
  return _pid.update(BalanceControl::_measuredAngle, dt);
}

void BalanceControl::setAngle(const float & angle) {
  _pid.setSetpoint(angle);
}

#line 1 "c:\\Users\\pelle\\OneDrive - TEC\\3g\\El\\Eksamensprojekt\\Kode\\Pikachu.ino"
#include <Arduino.h>
#include "EncoderMotor.h"
#include "Remote.h"
#include "Timer.h"
#include "MPU6050.h"
#include "BalanceControl.h"
#include <math.h>

void leftMotorInterruptHandler();
void rightMotorInterruptHandler();

EncoderMotor leftMotor(10, 6, 7, 2, 64 * 3, leftMotorInterruptHandler, 1, 0.001, 0.1);
EncoderMotor rightMotor(11, 4, 5, 3, 64 * 3, rightMotorInterruptHandler, 1, 0.001, 0.1);

void update(const unsigned int & dt);
Timer mainLoopTimer(10, update);

PID speedController(2., 5., 1.);

void leftMotorInterruptHandler() {
  leftMotor.interrupt();
}

void rightMotorInterruptHandler() {
  rightMotor.interrupt();
}

void setup() {
  Serial.begin(115200);

  Serial.print("Running at "); Serial.print(1000./mainLoopTimer.getInterval()); Serial.println("Hz");

  MPU6050::init();
  BalanceControl::init();

  BalanceControl::setAngle(0);
  speedController.setSetpoint(0);
  speedController.setLimits(-PI/8, PI/8);

  leftMotor.setDirection(1);
  rightMotor.setDirection(1);

  pinMode(A0, OUTPUT);
  pinMode(13, OUTPUT);
}

MPU6050::int16Vec3 accVec;
float angle;
float sinAngleTimesG;
float angularAcceleration;
float averageSpeed;
float totalSpeed = 0;
float rightSpeed;
float leftSpeed;
void update(const unsigned int & dt) {
  digitalWrite(A0, HIGH);

  // Fjernstyring
  Remote::update();

  
  // TODO: Hastighedsstyring
  averageSpeed = (leftMotor.getSpeed() + rightMotor.getSpeed()) / 2;
  //BalanceControl::setAngle(speedController.update(averageSpeed, dt));
  
  // Balancestyring
  MPU6050::update();
  accVec = MPU6050::getAccel();
  angle = atan2(accVec.y, accVec.z);
  //sinAngleTimesG = accVec.y; // y acceleration will be linearly proportional to sin of the angle
  totalSpeed = BalanceControl::update(angle, dt); // sin angle er måske ikke nødvendigt, test med og uden

  // Biasing
  rightSpeed = ((float)Remote::getState().steering + 1024) / 2048 * 2 * totalSpeed;
  leftSpeed = totalSpeed - rightSpeed;

  // Motorstyring
  //leftMotor.setSpeed(abs(leftSpeed));
  rightMotor.setSpeed(abs(rightSpeed));

  if(totalSpeed > 0) {
    leftMotor.setDirection(1);
    rightMotor.setDirection(1);
  } else {
    leftMotor.setDirection(-1);
    rightMotor.setDirection(-1);
  }

  Serial.println(totalSpeed);
  analogWrite(10, abs(totalSpeed));
  analogWrite(11, abs(totalSpeed));

  leftMotor.update(dt);
  rightMotor.update(dt);

  digitalWrite(A0, LOW);
}

void loop() {
  /*
  timeToNext = mainLoopTimer.update() - millis();
  if(timeToNext > 1) {
    delay(0.9 * timeToNext);
  }
  */

  if(mainLoopTimer.update() < millis()) {
    digitalWrite(13, HIGH); // brug den indbyggede LED til at indikere hvorvidt timingen er overskredet
  } else {
    digitalWrite(13, LOW);
  }
}
