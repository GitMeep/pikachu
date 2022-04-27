#include "BalanceControl.h"

float BalanceControl::_measuredAngle = 0;
PID BalanceControl::_pid(3, 0, 0);

void BalanceControl::init() {
  _pid.setLimiting(false);
}

float BalanceControl::update(const float & angle, const float & dt) {
  _measuredAngle += 0.6 * (angle - _measuredAngle);
  return _pid.update(BalanceControl::_measuredAngle, dt);
}

void BalanceControl::setAngle(const float & angle) {
  _pid.setSetpoint(angle);
}
