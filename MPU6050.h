#pragma once

class MPU6050 {
public:
  struct int16Vec3 {
    int16_t x, y, z;
  };

  static void init();
  static void update();
  static int16Vec3 getAccel();

private:
  const static int _I2CAddress=0x68;
  static int16_t _AcX, _AcY, _AcZ, _GyX, _GyY, _GyZ;

};