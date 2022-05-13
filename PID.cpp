#include "PID.h"
#include <Arduino.h>

PID::PID(const float & kP, const float & kI, const float & kD)
: _kP(kP), _kI(kI), _kD(kD),
_lastInput(0.),
_integratedError(0.),
_filteredDerivative(0.)
{}

// opdater PID regulator
float PID::update(const float & input, const float & dt) {
  float integ;
  _output = 0; // nulstil output
  _error = _setpoint - input; // beregn fejlen
    
  // proportionelt led
  _output += _kP * _error;

  // integral led
  _integratedError += _error * dt; // numerisk integration af fejlen
  integ = _integratedError;
  _output += _kI * integ; // læg integral-ledet til outputtet

  // afledt led
  _filteredDerivative += 0.2 * (((input - _lastInput) / dt) - _filteredDerivative); // lavpas filter på afledt led fordi den er sensitiv overfor støj
  _output -= _kD * _filteredDerivative; // læg differential-ledet til outputtet (gang med 1000 for at få mere håndterbare værdier)
  _lastInput = input; // opdater sidste fejl

  //Serial.print(_error); Serial.print(','); Serial.print(integ); Serial.print(','); Serial.println(0.1*_filteredDerivative);

  return _output;
}

// set setpoint :)
void PID::setSetpoint(const float & setpoint) {
  _setpoint = setpoint;
}

// tilføj en værdi til setpoint
void PID::addSetpoint(const float & difference) {
  _setpoint += difference;
}
