#include <Arduino.h>
#include "Ultrasound.h"
#include "EncoderMotor.h"
#include "Remote.h"
#include "Timer.h"
#include "BalanceControl.h"
#include "Display.h"
#include <math.h>
#include <MPU6050.h>

void leftMotorInterruptHandler();
void rightMotorInterruptHandler();

const byte soundPin = 8;
const byte projectilePin = 11;

EncoderMotor rightMotor = EncoderMotor(9, 6, 7, 2, 64 * 5, leftMotorInterruptHandler, 1, 0, 0);
EncoderMotor leftMotor = EncoderMotor(10, 4, 5, 3, 64 * 5, rightMotorInterruptHandler, 1, 0, 0);

void update(const float & dt);
//void balancePID(const float & dt);
//Timer balancePIDTimer(10, balancePID);
Timer mainLoopTimer(20, update);

/*
MPU6050 mpu;
int16_t accZ, accX, gyroRate;
const float sp = 1.8;
*/

void playSound() {
  digitalWrite(soundPin, HIGH);
  digitalWrite(soundPin, LOW);
}

void setup() {
  Serial.begin(115200);

  //Serial.println("Error,Integral,Derivative");

  /*
  mpu.initialize();
  mpu.setZAccelOffset(920);
  mpu.setXAccelOffset(-1515);
  mpu.setYGyroOffset(-47);

  mpu.setRate(0);
  mpu.setDLPFMode(MPU6050_DLPF_BW_256);
  mpu.setClockSource(MPU6050_CLOCK_PLL_YGYRO);
  */

  //BalanceControl::setAngle(sp);
  //speedController.setSetpoint(0);
  //speedController.setLimits(-PI/8, PI/8);

  leftMotor.setSpeed(0);
  rightMotor.setSpeed(0);

  Remote::init();
  Ultrasound::init();
  Display::init();
  Display::select(Display::AMONGUS);

  pinMode(13, OUTPUT);
  pinMode(soundPin, OUTPUT);
  pinMode(projectilePin, OUTPUT);

  digitalWrite(soundPin, LOW);
  digitalWrite(projectilePin, LOW);
}

bool resetTimerEnabled = false;
unsigned long resetTime = 0;

int totalSpeed, rightSpeed, leftSpeed;
void update(const float & dt) {
  // Fjernstyring
  Remote::update();
  Ultrasound::ping();

  auto remoteState = Remote::getState();
  totalSpeed = ((int)remoteState.direction) * remoteState.power/2;
  
  if(remoteState.fire) {
    Display::select(Display::TARGET);
    digitalWrite(projectilePin, HIGH);
    playSound();
  } else {
    digitalWrite(projectilePin, LOW);
  }

  unsigned int ultrasoundDistance = Ultrasound::getDistance();
  if(ultrasoundDistance < 300 && remoteState.direction == 1) {
    totalSpeed = 0;
  }

  if(ultrasoundDistance < 280) {
    totalSpeed = -150;
    Display::select(Display::ANGRY);
    resetTime = millis() + 5000;
    resetTimerEnabled = true;
    playSound();
  }

  if(millis() >= resetTime && resetTimerEnabled) {
    Display::select(Display::NEUTRAL);
    resetTimerEnabled = false;
  }

  // Biasing
  rightSpeed = totalSpeed * (512 + remoteState.steering) / 1200;
  leftSpeed = totalSpeed - rightSpeed;

  // Motorstyring
  leftMotor.setSpeed(leftSpeed);
  rightMotor.setSpeed(rightSpeed);
}

/*
float accAngle, dgyroAngle;
float gyroY;
const float gyroWeight = 0.9;
float angle = 0;
float angularAcceleration;
float totalSpeed = 0;
void balancePID(const float & dt) {
  // Balancestyring
  accZ = mpu.getAccelerationZ();
  accX = mpu.getAccelerationX();

  gyroY = mpu.getRotationY();
  
  gyroRate = gyroY / 131.068; // map from +-32767 to +-250

  accAngle = atan2(accX, accZ)*RAD_TO_DEG;
  dgyroAngle = gyroRate * dt;

  angle = gyroWeight * (angle + dgyroAngle) + (1 - gyroWeight) * (accAngle);

  totalSpeed = BalanceControl::update(angle, dt);
  if(sp-1 < angle && angle < sp+1) { // deadzone
    totalSpeed = 0;
  }
}
*/

void loop() {
  if(mainLoopTimer.update() - 2 > millis()) {
    Display::drawRow(); // tegn på displayet hvis der er nok tid
  }
}