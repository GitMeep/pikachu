#include <Arduino.h>
#include <avr/pgmspace.h>
#include "Display.h"

// pins til skifteregistre
const byte latchPin = A0;
const byte clockPin = A1;
const byte dataPin = A2;

// ansigter opbevares i FLASH hukommelsen, de kopieres ind i SRAM linje pr. linje når de skal bruges.
const PROGMEM uint16_t expressions[][16] = {
  { // NEUTRAL
    0b1111111111111111,
    0b1111111111111111,
    0b1100001111000011,
    0b1100001111000011,
    0b1111111111111111,
    0b1111111111111111,
    0b1111111111111111,
    0b1111111111111111,
    0b1111111111111111,
    0b1111111111111111,
    0b1111111111111111,
    0b1100111111110011,
    0b1100111111110011,
    0b1111000000001111,
    0b1111000000001111,
    0b1111111111111111
  },
  { // TARGET
    0b1111000000001111,
    0b1111000000001111,
    0b1100111111110011,
    0b1100111111110011,
    0b0011111111111100,
    0b0011111111111100,
    0b0011111111111100,
    0b0011111001111100,
    0b0011111001111100,
    0b0011111111111100,
    0b0011111111111100,
    0b0011111111111100,
    0b1100111111110011,
    0b1100111111110011,
    0b1111000000001111,
    0b1111000000001111
  },
  { // ANGRY
    0b1111111111111111,
    0b1111111111111111,
    0b1100111111110011,
    0b1100111111110011,
    0b1111001111001111,
    0b1111001111001111,
    0b1111111111111111,
    0b1111111111111111,
    0b1111111111111111,
    0b1111111111111111,
    0b1111111111111111,
    0b1111000000001111,
    0b1111000000001111,
    0b1100111111110011,
    0b1100111111110011,
    0b1111111111111111
  },
  { // AMONGUS
    0b1111111111111111,
    0b1111111111111111,
    0b1111000000001111,
    0b1111000000001111,
    0b1100000000001111,
    0b1100000011111111,
    0b1100000011111111,
    0b1100000000001111,
    0b1100000000001111,
    0b1100000000001111,
    0b1111000000001111,
    0b1111000000001111,
    0b1111001111001111,
    0b1111001111001111,
    0b1111001111001111,
    0b1111001111001111
  }
};

// alloker og initialiser statiske medlemmer
uint16_t Display::_rowSelect = 1;       // en bit-maske som holder styr på hvilken række der skal tegnes
uint8_t Display::_selectedRow = 0;      // indeks af rækken der skal tegnes
uint8_t Display::_selectedPicture = 0;  // indeks af det valgte billede

// klargør displayet
void Display::init() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

// vælg et ansigt
void Display::select(Expression expression) {
  _selectedPicture = expression;
}

// Tegn en række. Displayet virker ved at scanne ned over det, og tegne en række ad gangen. Denne
// funktion kaldes så ofte som muligt ind imellem andre opdateringer sker.
void Display::drawRow() {
  _rowSelect = _rowSelect == 0 ? 1 : _rowSelect; // nulstil rowSelect hvis bit'en er blevet skiftet ud af enden

  uint16_t row; // midlertidig variabel til at holde den række som skal tegnes
  memcpy_P(&row, expressions[_selectedPicture] + _selectedRow, sizeof(uint16_t)); // kopier rækken fra FLASH til SRAM

  // Skift bits ud på registre. Første 16 bit vælger rækken, næste 16 bits angiver mønster
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, (row >> 8));
  shiftOut(dataPin, clockPin, MSBFIRST, row);
  shiftOut(dataPin, clockPin, LSBFIRST, _rowSelect);
  shiftOut(dataPin, clockPin, LSBFIRST, (_rowSelect >> 8));
  digitalWrite(latchPin, HIGH);
  
  _rowSelect = _rowSelect << 1;       // flyt bit en til venstre for at vælge næste række
  _selectedRow = (++_selectedRow)%16; // inkrementer valgte række og nulstil hvis den når over 15
}