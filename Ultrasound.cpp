#include <Arduino.h>
#include "Ultrasound.h"

#define SPEED_OF_SOUND_TIMES_2 2 * 0.340 // mm/us

// alloker og initialiser statiske medlemmer
volatile unsigned int Ultrasound::_distance = 500;
volatile bool Ultrasound::_pinging = false;
const uint8_t echoPin = 3;
const uint8_t trigPin = 12;

unsigned long Ultrasound::_outTime; // in us

// set interrupt og pins op
void Ultrasound::init() {
  attachInterrupt(digitalPinToInterrupt(echoPin), Ultrasound::interrupt, FALLING);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}

// udsend en puls hvis ikke vi allerede venter en
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

// få den sidst-målte distance
unsigned int Ultrasound::getDistance() {
  return _distance;
}

// Kaldes når en puls kommer tilbage. Hvis vi ikke har sendt en puls ud er det falsk alarm, ellers opdateres den målte distance.
void Ultrasound::interrupt() {
  if(!_pinging) return;
  _distance = (micros() - _outTime) / SPEED_OF_SOUND_TIMES_2;
  _pinging = false;
}