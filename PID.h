#pragma once

/*
* En PID regulator.
*/

class PID {
public:
  PID(const float & kP, const float & kI, const float & kD);

  void setSetpoint(const float & setpoint);
  void addSetpoint(const float & difference);
  float update(const float & input, const float & dt);

private:
  // PID parametre
  float _kP, _kI, _kD;
  float _setpoint;

  // variabler der skal bruges under beregninger, så de ikke skal allokeres hver gang
  float _error;   // fejl mellem måling og setpoint
  float _output;  // outputtet

  // infinitessimalregning-ting
  float _filteredDerivative;
  float _lastInput;       // sidste måling, bruges til at beregne den tidsafledte
  float _integratedError; // integrerede fejl, bruges i integral ledet
};