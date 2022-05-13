#pragma once

/*
* Klasse til at håndtere en ultralyds-sensor asynkront. I stedet for at bruge Arduino's pulseIn funktion, gemmes tiden
* hvor en puls blev sendt ud, og et interrupt detekterer hvornår pulsen kom tilbage.
*/
class Ultrasound {
public:
  static void init(); // intialiser pins og interrupt

  static void ping(); // udsend en ultralydspuls
  static unsigned int getDistance(); // få den senest-målte distance

  static void interrupt(); // funktion der bliver kaldt når pulsen er kommet tilbage

protected:
  volatile static bool _pinging; // hvorvidt vi er i gang med at vente på at en puls kommer tilbage
  volatile static unsigned int _distance; // den senest-målte distance
  static unsigned long _outTime; // hvornår pulsen blev sendt ud i mikrosekunder

};