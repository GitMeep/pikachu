#line 1 "c:\\Users\\pelle\\OneDrive - TEC\\3g\\El\\Eksamensprojekt\\Kode\\MPU6050.cpp"
#include <Arduino.h>
#include <Wire.h>
#include "MPU6050.h"

int16_t MPU6050::_AcX, MPU6050::_AcY, MPU6050::_AcZ;
int16_t MPU6050::_GyX, MPU6050::_GyY, MPU6050::_GyZ;

// adapteret fra https://create.arduino.cc/projecthub/Nicholas_N/how-to-use-the-accelerometer-gyroscope-gy-521-6dfc19
void MPU6050::init() {
  Wire.begin();
  Wire.beginTransmission(_I2CAddress);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

void MPU6050::update() {
  Wire.beginTransmission(_I2CAddress);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(_I2CAddress, 6, true);
  _AcX=Wire.read()<<8|Wire.read();
  _AcY=Wire.read()<<8|Wire.read();
  _AcZ=Wire.read()<<8|Wire.read();
}

MPU6050::int16Vec3 MPU6050::getAccel() {
    return {_AcX, _AcY, _AcZ};
  }