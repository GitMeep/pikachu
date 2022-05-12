#include "BalanceControl.h"

// https://en.wikipedia.org/wiki/PID_controller#Ziegler%E2%80%93Nichols_method
const float Ku = 18;
const float Tu = 1;

float BalanceControl::_measuredAngle = 0;
PID BalanceControl::_pid(0.6 * Ku, 1.4*Ku/Tu, 3.*Ku*Tu/40);

// opdater PID regulator
float BalanceControl::update(const float & angle, const float & dt) {
  _measuredAngle = angle;
  return _pid.update(BalanceControl::_measuredAngle, dt);
}

// sæt vinkelen som robotten prøver at holde
void BalanceControl::setAngle(const float & angle) {
  _pid.setSetpoint(angle);
}
