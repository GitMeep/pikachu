#include <Arduino.h>
#include "Timer.h"

// Konstruer timer. Første kørsel er efter èt interval.
Timer::Timer(unsigned long interval, TimerHandler handler)
: _interval(interval),
_handler(handler),
_lastRun(millis()) {
    _nextRun = _lastRun + _interval;
}

// kører handler funktionen hvis det er tid
const unsigned long & Timer::update() {
    _thisRun = millis(); // gem den nuværende tid, sådan at den ikke ændrer sig midt i funktionen
    if(_thisRun >= _nextRun) { // hvis vi er nået til at skulle køre funktionen
        _nextRun += _interval; // opdater tiden hvor funktionen skal køres næste gang
        _handler(((float)(_thisRun - _lastRun))/1000); // kør funktionen, og passer tiden i sekunder siden sidste kørsel
        _lastRun = _thisRun; // gem hvornår funktionen blev kørt, til brug i linjen herover
    }

    return _nextRun; // returner hvornår funktionen skal køres næste gang
}

// hvor ofte skal handler-funktionen køres?
const unsigned long & Timer::getInterval() {
    return _interval;
}