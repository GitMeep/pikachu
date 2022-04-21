#line 1 "c:\\Users\\pelle\\OneDrive - TEC\\3g\\El\\Eksamensprojekt\\Kode\\EncoderMotor.h"
#pragma once
#include "PID.h"

class EncoderMotor {
public:
  EncoderMotor(const uint8_t motorPin, const uint8_t pinA, const uint8_t pinB, const uint8_t encoderPin, const float & encoderCpr, void (*encoderInt)(), const float & Kp, const float & Ki, const float & Kd);

  void interrupt();
  void update(const unsigned int & dt);
  void setDirection(const int8_t & direction);
  void setSpeed(const float & speed);
  float getSpeed();

private:
  unsigned long _position;
  unsigned long _lastPosition;
  unsigned long _currentPosition;

  float _filteredSpeed;
  int8_t _direction;
  float _cpr;

  uint8_t _motorPin;
  uint8_t _pinA;
  uint8_t _pinB;

  PID _pid;

};