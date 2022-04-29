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
  attachInterrupt(digitalPinToInterrupt(encoderPin), encoderInt, FALLING);

  analogWrite(_motorPin, 0);

  pinMode(_pinA, OUTPUT);
  pinMode(_pinB, OUTPUT);

  setSpeed(0);
}

void EncoderMotor::interrupt() {
  _measuredPosition += _direction;
}

void EncoderMotor::update(const float & dt) {
  _currentPosition = _measuredPosition;
  _pid.setSetpoint(_targetPosition);
  _targetPosition += _speed * _cpr * dt;

  //analogWrite(_motorPin, _pid.update(_currentPosition, dt));
}

// target speed in revolutions per second
void EncoderMotor::setSpeed(const float & speed) {
  _speed = abs(speed);

  analogWrite(_motorPin, _speed);

  if(speed == 0) _direction = 0;
  if(speed < 0) {
    _direction = -1;
    digitalWrite(_pinA, LOW);
    digitalWrite(_pinB, HIGH);
  } else {
    _direction = 1;
    digitalWrite(_pinA, HIGH);
    digitalWrite(_pinB, LOW);
  }
}

void EncoderMotor::setTargetPosition(const long & position) {
  _targetPosition = position;
}

long EncoderMotor::getTargetPosition() {
  return _targetPosition;
}

float EncoderMotor::getSpeed() {
  return _speed;
}