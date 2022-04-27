#include <Arduino.h>
#include "Encodermotor.h"
#include "Remote.h"
#include "Timer.h"
#include "MPU6050.h"
#include "BalanceControl.h"
#include <math.h>

void leftMotorInterruptHandler();
void rightMotorInterruptHandler();

EncoderMotor leftMotor = EncoderMotor(10, 6, 7, 2, 64 * 3, leftMotorInterruptHandler, 1, 0, 0);
EncoderMotor rightMotor = EncoderMotor(11, 4, 5, 3, 64 * 3, rightMotorInterruptHandler, 1, 0, 0);

void update(const float & dt);
Timer mainLoopTimer(10, update);

PID speedController(2., 5., 1.);

void leftMotorInterruptHandler() {
  //leftMotor.interrupt();
}

void rightMotorInterruptHandler() {
  //rightMotor.interrupt();
}

void setup() {
  Serial.begin(9600);

  Serial.print("Running at "); Serial.print(1000./mainLoopTimer.getInterval()); Serial.println("Hz");

  MPU6050::init();
  BalanceControl::init();

  BalanceControl::setAngle(0);
  speedController.setSetpoint(0);
  speedController.setLimits(-PI/8, PI/8);

  leftMotor.setSpeed(0);
  rightMotor.setSpeed(0);

  pinMode(A0, OUTPUT);
  pinMode(13, OUTPUT);
}

MPU6050::int16Vec3 accVec, gyVec;
float angle;
float sinAngleTimesG;
float angularAcceleration;
float averageSpeed;
float totalSpeed = 0;
float rightSpeed;
float leftSpeed;
void update(const float & dt) {
  digitalWrite(A0, HIGH);

  // Fjernstyring
  Remote::update();

  // TODO: Hastighedsstyring
  averageSpeed = (leftMotor.getSpeed() + rightMotor.getSpeed()) / 2;
  //BalanceControl::setAngle(speedController.update(averageSpeed, dt));
  
  // Balancestyring
  MPU6050::update();
  accVec = MPU6050::getAccel();
  gyVec = MPU6050::getGyro();
  angle = (0.98 * angle + ) atan2(accVec.y, accVec.z);
  //sinAngleTimesG = accVec.y; // y acceleration will be linearly proportional to sin of the angle
  totalSpeed = BalanceControl::update(angle, dt); // sin angle er måske ikke nødvendigt, test med og uden

  // Biasing
  rightSpeed = totalSpeed * (1024. + Remote::getState().steering) / 2048.;
  leftSpeed = totalSpeed - rightSpeed;

  Serial.println(angle);

  // Motorstyring
  leftMotor.setSpeed(leftSpeed);
  rightMotor.setSpeed(rightSpeed);

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