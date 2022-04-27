#pragma once

#include "PID.h"

/*
* Denne klasses opgave er at holde robotten ved en specifik vinkel ved hjælp
* af et accelermoeter og en PID regulator.
*/
class BalanceControl {
public:
  static void init();

  static float update(const float & angle, const float & dt);
  static void setAngle(const float & angle);

private:
  static float _measuredAngle;
  static PID _pid;
};