#pragma once

/*
* En PID regulator.
*/

class PID {
public:
  PID(const float & kP, const float & kI, const float & kD);

  void setSetpoint(const float & setpoint); // set setpoint :)
  void addSetpoint(const float & difference); // tilføj en værdi til setpoint
  float update(const float & input, const float & dt); // opdater PID regulator

private:
  // PID parametre
  float _kP, _kI, _kD;
  float _setpoint;

  // variabler der skal bruges under beregninger, så de ikke skal allokeres hver gang
  float _error;   // fejl mellem måling og setpoint
  float _output;  // outputtet

  // infinitessimalregning-ting
  float _filteredDerivative; // det afledte led er følsomt overfor støj, så den lavpas-filtreres
  float _lastInput;       // sidste måling, bruges til at beregne den tidsafledte
  float _integratedError; // integrerede fejl, bruges i integral ledet
};