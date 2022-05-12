#pragma once

class Ultrasound {
public:
  Ultrasound(const uint8_t trigPin, const uint8_t echoPin, void (*intHandler)());

  static void init();

  static void ping();
  static unsigned int getDistance();

  static void interrupt();

protected:
  volatile static bool _pinging;
  volatile static unsigned int _distance;
  static unsigned long _outTime; // in us

};