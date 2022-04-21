#line 1 "c:\\Users\\pelle\\OneDrive - TEC\\3g\\El\\Eksamensprojekt\\Kode\\PID.h"
#pragma once

class PID {
public:
    PID(const float & kP, const float & kI, const float & kD);

    void setSetpoint(const float & setpoint);
    void addSetpoint(const float & difference);
    void setLimits(const float & minOut, const float & maxOut);
    float update(const float & input, const unsigned int & dt);

private:
    // PID parametre
    float _kP, _kI, _kD;
    float _setpoint;

    float _minOut, _maxOut;

    // variabler der skal bruges under beregninger, så de ikke skal allokeres hver gang
    float _error; // fejl mellem måling og setpoint
    float _output; // outputtet :)

    // infinitessimalregning-ting
    float _lastError; // sidste fejl, bruges til at beregne den tidsafledte
    float _integratedError; // integrerede fejl, bruges i integral ledet
};