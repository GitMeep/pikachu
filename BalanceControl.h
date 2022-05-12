#pragma once

#include "PID.h"

/*
* Denne klasses opgave er at holde robotten ved en specifik vinkel ved hjælp
* af en PID regulator som får en vinkel udefra.
*/
class BalanceControl {
public:
  static float update(const float & angle, const float & dt);
  static void setAngle(const float & angle);

private:
  static float _measuredAngle;
  static PID _pid;
};