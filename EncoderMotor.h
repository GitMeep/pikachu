#pragma once
#include "PID.h"

/*
* Denne klasse håndterer en motor-encoder kombination. Vi endte dog ikke
* med at bruge encoderne, så den sætter faktisk bare hastighed og retning
* uden noget feedback.
*/

class EncoderMotor {
public:
  EncoderMotor(const uint8_t motorPin, const uint8_t pinA, const uint8_t pinB, const uint8_t encoderPin, const float & encoderCpr, void (*encoderInt)(), const float & Kp, const float & Ki, const float & Kd);

  void setSpeed(const float & speed);
  float getSpeed();

private:
  long _targetPosition;             // øjeblikkelig 
  float _speed;                     // ønsket hastighed i rotationer per sekund
  char _direction;                  // encoderene har ingen måde at måle retning på, så det holdes styr på internt
  volatile long _measuredPosition;  // position målt af encodere
  long _currentPosition;            // brugt til at lave en kopi af den målte position under hver opdatering

  float _cpr;                       // antal pulser pr. rotation fra encoderen

  uint8_t _motorPin;                // motorens enable pin på H-broen
  uint8_t _pinA;                    // motorens A pin på H-broen
  uint8_t _pinB;                    // motorens B pin på H-broen

  PID _pid;                         // PID regulator til at styre motorkraft

};