#include <Arduino.h>
#include <avr/pgmspace.h>
#include "Display.h"

const byte latchPin = A0;
const byte clockPin = A1;
const byte dataPin = A2;

// opbevar ansigter i FLASH hukommelsen, de kopieres ind i SRAM linje pr. linje når de skal bruges.
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

uint16_t Display::_rowSelect = 1;
uint8_t Display::_selectedRow = 0;
uint8_t Display::_selectedPicture = 0;

void Display::init() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void Display::select(Expression expression) {
  _selectedPicture = expression;
}

void Display::drawRow() {
  _rowSelect = _rowSelect == 0 ? 1 : _rowSelect;

  uint16_t row;
  memcpy_P(&row, expressions[_selectedPicture] + _selectedRow, sizeof(uint16_t));

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, (row >> 8));
  shiftOut(dataPin, clockPin, MSBFIRST, row);
  shiftOut(dataPin, clockPin, LSBFIRST, _rowSelect);
  shiftOut(dataPin, clockPin, LSBFIRST, (_rowSelect >> 8));
  digitalWrite(latchPin, HIGH);
  
  _rowSelect = _rowSelect << 1;
  _selectedRow = (++_selectedRow)%16;
}