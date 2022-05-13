#pragma once

/*
* Denne klasse holder styr på hvad displayet skal vise, og er ansvarlig
* for at tegne det.
*/
class Display {
public:
  // enum over de mulige ansigtsudtryk, så man ikke behøver at huske deres indeks
  enum Expression {
    NEUTRAL,
    //SAD,
    TARGET,
    ANGRY,
    //QUESTION,
    //ALERT,
    AMONGUS
  };

  static void init(); // initialiser displayet
  static void select(Expression expression); // vælg et ansigtsudtryk

  static void drawRow(); // Tegn næste række. Denne kaldes så ofte som muligt.

private:
  static uint16_t _rowSelect; // bit-maske til at vælge række
  static uint8_t _selectedRow; // tal der holde styr på indekset af den række vi har gang i
  static uint8_t _selectedPicture; // indeks af det valge billede

};