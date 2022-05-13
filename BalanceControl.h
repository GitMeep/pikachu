#pragma once

#include "PID.h"

/*
* Denne klasses opgave er at holde robotten ved en specifik vinkel ved hjælp
* af en PID regulator som får en vinkel udefra.
*/
class BalanceControl {
public:
  static float update(const float & angle, const float & dt); // opdaterer PID regulatoren
  static void setAngle(const float & angle); // sæt den ønskede vinkel

private:
  static float _measuredAngle; // målt vinkel
  static PID _pid; // PID objekt
};