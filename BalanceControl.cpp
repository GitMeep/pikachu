#include "BalanceControl.h"

float BalanceControl::_measuredAngle = 0;
PID BalanceControl::_pid(50, 10, 0);

void BalanceControl::init() {
  _pid.setLimits(-255, 255);
}

float BalanceControl::update(const float & angle, const unsigned int & dt) {
  _measuredAngle += 0.6 * (angle - _measuredAngle);
  return _pid.update(BalanceControl::_measuredAngle, dt);
}

void BalanceControl::setAngle(const float & angle) {
  _pid.setSetpoint(angle);
}
