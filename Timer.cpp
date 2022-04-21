#include <Arduino.h>
#include "Timer.h"

Timer::Timer(unsigned long interval, TimerHandler handler)
: _interval(interval),
_handler(handler),
_lastRun(millis()),
_nextRun(_lastRun + _interval)
{}

const unsigned long & Timer::update() {
    _thisRun = millis();
    if(_thisRun >= _nextRun) {
        _nextRun += _interval;
        _handler(_thisRun - _lastRun);
        _lastRun = _thisRun;
    }

    return _nextRun;
}

const unsigned long & Timer::getInterval() {
    return _interval;
}