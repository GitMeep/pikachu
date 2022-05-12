#include "BalanceControl.h"

const float Ku = 18;
const float Tu = 1;

float BalanceControl::_measuredAngle = 0;
PID BalanceControl::_pid(0.6 * Ku, 1.4*Ku/Tu, 10.*Ku*Tu/40);

float BalanceControl::update(const float & angle, const float & dt) {
  _measuredAngle = angle;
  return _pid.update(BalanceControl::_measuredAngle, dt);
}

void BalanceControl::setAngle(const float & angle) {
  _pid.setSetpoint(angle);
}
