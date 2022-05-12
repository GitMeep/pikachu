#include <Arduino.h>
#include "EncoderMotor.h"

EncoderMotor::EncoderMotor(const uint8_t motorPin, const uint8_t pinA, const uint8_t pinB, const uint8_t encoderPin, const float & encoderCpr, void (*encoderInt)(), const float & Kp, const float & Ki, const float & Kd)
  : _motorPin(motorPin), _pinA(pinA), _pinB(pinB),
  _targetPosition(0),
  _measuredPosition(0),
  _cpr(encoderCpr),
  _speed(0),
  _direction(1),
  _pid(Kp, Ki, Kd) {
  //attachInterrupt(digitalPinToInterrupt(encoderPin), encoderInt, FALLING);

  pinMode(_pinA, OUTPUT);
  pinMode(_pinB, OUTPUT);

  // sørg for at motorene ikke kører
  setSpeed(0);
}

// set fart og retning
void EncoderMotor::setSpeed(const float & speed) {
  _speed = abs(speed);
  analogWrite(_motorPin, constrain(_speed, 0., 255.));

  if(speed < 0) {
    _direction = -1;
    digitalWrite(_pinA, 0);
    digitalWrite(_pinB, 1);
  } else if(speed > 0) {
    _direction = 1;
    digitalWrite(_pinA, 1);
    digitalWrite(_pinB, 0);
  } else { // speed == 0
    digitalWrite(_pinA, 0);
    digitalWrite(_pinB, 0);
  }
}

float EncoderMotor::getSpeed() {
  return _speed;
}