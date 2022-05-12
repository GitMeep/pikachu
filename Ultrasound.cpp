#include <Arduino.h>
#include "Ultrasound.h"

#define SPEED_OF_SOUND_TIMES_2 2 * 0.340 // mm/us

volatile unsigned int Ultrasound::_distance = 500;
volatile bool Ultrasound::_pinging = false;
const uint8_t echoPin = 3;
const uint8_t trigPin = 12;

unsigned long Ultrasound::_outTime; // in us

void Ultrasound::init() {
  attachInterrupt(digitalPinToInterrupt(echoPin), Ultrasound::interrupt, FALLING);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}

void Ultrasound::ping() {
  if(_pinging) return;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  _outTime = micros();
  _pinging = true;
}

unsigned int Ultrasound::getDistance() {
  return _distance;
}

void Ultrasound::interrupt() {
  _distance = (micros() - _outTime) / SPEED_OF_SOUND_TIMES_2;
  _pinging = false;
}