#include "PID.h"
#include <Arduino.h>

PID::PID(const float & kP, const float & kI, const float & kD)
: _kP(kP), _kI(kI), _kD(kD),
_minOut(40.), _maxOut(255.),
_lastError(0.),
_integratedError(0.),
_enableLimits(true)
{}

float PID::update(const float & input, const float & dt) {
    _output = 0; // nulstil output
    _error = _setpoint - input; // beregn error
    
    // proportionel
    _output += _kP * _error; // læg det proportionelle led til outputtet

    // integral
    _integratedError += _error * dt; // numerisk integration af fejlen
    _output += _kI * _integratedError; // læg integral-ledet til outputtet

    if(isnan(_integratedError)) _integratedError = 0;

    // derivative
    _output += _kD * (_error - _lastError) / dt; // læg differential-ledet til outputtet (gang med 1000 for at få mere håndterbare værdier)
    _lastError = _error; // opdater sidste fejl

    if(_enableLimits) return constrain(_output, _minOut, _maxOut);
    return _output;
}

void PID::setSetpoint(const float & setpoint) {
    _setpoint = setpoint;
}

void PID::addSetpoint(const float & difference) {
    _setpoint += difference;
}

void PID::setLimits(const float & minOut, const float & maxOut) {
    _minOut = minOut;
    _maxOut = maxOut;
}

void PID::setLimiting(bool limiting) {
    _enableLimits = limiting;
}
