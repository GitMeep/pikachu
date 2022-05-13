#pragma once

/*
* Klasse som kan køre en funktion med en jævn frekvens. Det er ikke garanteret at funktionen bliver
* kaldt med et fast interval, men så længe at update() bliver kaldt med høj nok frekvens, vil
* funktionen blive kørt indenfor hvert interval. Dette er illustreret med følgende figur:
* | *      |*       |     *  |*       |   *    |   *    |  *     |
* Stjerne repræsenterer funktionen der bliver kalt, stregerne markerer intervallet. Så længe at
* update() bliver kaldt med højere frekvens end intervallerne kommer, burde funktionen blive kaldt
* med den rigtige gennemsnitlige frekvens.
*/
class Timer {
public:
    typedef void (*TimerHandler)(const float &); // typedef af funktion pointer til timerens handler funktion
    
    Timer(unsigned long interval, TimerHandler handler);

    const unsigned long & update(); // kører funktionen hvis det er tid, og returnerer hvornår næste kørsel tidligst sker
    const unsigned long & getInterval();

private:
    unsigned long _nextRun; // hvornår skal funktionen køres næste gang?
    unsigned long _interval; // hvor lang tid skal der gå imellem at funktionen køres?
    TimerHandler _handler; // funktion pointer til funktionen. Tiden siden sidste kørsel i sekunder passeres til funktionen.
    unsigned long _lastRun; // bruges til at beregne hvor lang tid der rent faktisk er gået siden sidste kørsel
    unsigned long _thisRun; // variabel der bruges til at holde styr på hvornår et "run" faktisk startede
};