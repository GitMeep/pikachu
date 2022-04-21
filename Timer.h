#pragma once

/*
* Klasse som kan køre en funktion med en jævn frekvens. Det er ikke garanteret at funktionen bliver
* kaldt med et fast interval, men så længe at update() bliver kaldt med højere frekvens, vil
* funktionen blive kørt indenfor hvert interval. Dette er illustreret med følgende figur:
* | *      |*       |     *  |*       |   *    |   *    |  *     |
* Stjerne repræsenterer funktionen der bliver kalt, stregerne markerer intervallet. Så længe at
* update() bliver kaldt med meget højere frekvens end 
*/
class Timer {
public:
    typedef void (*TimerHandler)(const unsigned int &);
    
    Timer(unsigned long interval, TimerHandler handler);

    const unsigned long & update(); // kører funktionen hvis det er tid, og returnerer hvornår næste kørsel tidligst sker
    const unsigned long & getInterval();

private:
    unsigned long _nextRun; // hvornår skal funktionen køres næste gang?
    unsigned long _interval; // hvor lang tid skal der gå imellem at funktionen køres?
    TimerHandler _handler; // funktion pointer til funktionen. Antallet millisekunder siden sidste kørsel passeres til funktionen.
    unsigned long _lastRun; // bruges til at beregne hvor lang tid der rent faktisk er gået siden sidste kørsel
    unsigned long _thisRun; // variabel der bruges til at holde styr på hvornår et "run" faktisk startede
};