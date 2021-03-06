#pragma once

/*
* Klasse til at kommunikere med accelerometer/gyroskop.
* Endte med at bruge et tredjepartsbibliotek i stedet.
*/
class MPU6050 {
public:
  struct int16Vec3 {
    int16_t x, y, z;
  };

  static void init();
  static void update();
  static int16Vec3 getAccel();
  static int16Vec3 getGyro();

private:
  const static int _I2CAddress=0x68;
  static int16_t _AcX, _AcY, _AcZ, _GyX, _GyY, _GyZ;

};