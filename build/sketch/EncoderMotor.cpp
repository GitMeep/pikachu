#line 1 "c:\\Users\\pelle\\OneDrive - TEC\\3g\\El\\Eksamensprojekt\\Kode\\EncoderMotor.cpp"
#include <Arduino.h>
#include "EncoderMotor.h"

EncoderMotor::EncoderMotor(const uint8_t motorPin, const uint8_t pinA, const uint8_t pinB, const uint8_t encoderPin, const float & encoderCpr, void (*encoderInt)(), const float & Kp, const float & Ki, const float & Kd)
  : _motorPin(motorPin), _pinA(pinA), _pinB(pinB),
  _filteredSpeed(0),
  _cpr(encoderCpr),
  _position(encoderCpr),
  _lastPosition(0),
  _direction(1),
  _pid(Kp, Ki, Kd) {
  attachInterrupt(digitalPinToInterrupt(encoderPin), encoderInt, RISING);

  analogWrite(_motorPin, 0);

  pinMode(_pinA, OUTPUT);
  pinMode(_pinB, OUTPUT);
}

void EncoderMotor::interrupt() {
  _position += _direction;
}

void EncoderMotor::update(const unsigned int & dt) {
  _currentPosition = _position;
  _filteredSpeed += 0.1 * (((1000. * (_currentPosition - _lastPosition)) / dt) - _filteredSpeed);
  _lastPosition = _currentPosition;

  if(isnan(_filteredSpeed)) _filteredSpeed = 0;

  //analogWrite(_motorPin, _pid.update(_filteredSpeed, dt));
}

void EncoderMotor::setDirection(const int8_t & direction) {
  _direction = direction;

  if(direction == 0) setSpeed(0);
  if(direction < 0) {
    digitalWrite(_pinA, 0);
    digitalWrite(_pinB, 1);
  } else {
    digitalWrite(_pinA, 1);
    digitalWrite(_pinB, 0);
  }
}

// target speed in revolutions per second
void EncoderMotor::setSpeed(const float & speed) {
  _pid.setSetpoint(speed * _cpr);
}

float EncoderMotor::getSpeed() {
  return _filteredSpeed;
}