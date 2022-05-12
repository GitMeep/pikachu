#pragma once

/*
* Denne klasse holder styr på hvad displayet skal vise, og er ansvarlig
* for at tegne det.
*/
class Display {
public:
  enum Expression {
    NEUTRAL,
    //SAD,
    TARGET,
    ANGRY,
    //QUESTION,
    //ALERT,
    AMONGUS
  };

  static void init();
  static void select(Expression expression);

  static void drawRow();

private:
  static uint16_t _rowSelect;
  static uint8_t _selectedRow;
  static uint8_t _selectedPicture;

};