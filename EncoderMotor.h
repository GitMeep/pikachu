#pragma once
#include "PID.h"

class EncoderMotor {
public:
  EncoderMotor(const uint8_t motorPin, const uint8_t pinA, const uint8_t pinB, const uint8_t encoderPin, const float & encoderCpr, void (*encoderInt)(), const float & Kp, const float & Ki, const float & Kd);

  void setSpeed(const float & speed);
  float getSpeed();

private:
  long _targetPosition; // instantaneous target position
  float _speed; // speed in rotations per second
  char _direction;
  long _measuredPosition; // measured position
  long _currentPosition; // used to make a copy of the measured position in the beginning of each update

  float _cpr;

  uint8_t _motorPin;
  uint8_t _pinA;
  uint8_t _pinB;

  PID _pid;

};