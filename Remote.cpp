#include <Arduino.h>
#include "Remote.h"

unsigned long Remote::_lastSeen = 0;
bool Remote::_connected = false;
Remote::ControlState Remote::_state = {0, 0, 0, false};

void Remote::update() {
    if (Serial.available() >= sizeof(ControlState) + 1) {
        ControlState data;
        Serial.readBytes((char*)&data, sizeof(ControlState));
        Serial.find('\n');

        digitalWrite(7, HIGH);
        digitalWrite(7, LOW);

        if(data.power <= 1024 && data.steering <= 1024 && data.steering >= -1024 && (data.direction == 1 || data.direction == 0 || data.direction == -1)) {
            _connected = true;
            Remote::_state = data;
            _lastSeen = millis();
        }
    }

    if(millis() - _lastSeen > _timeout) {
        _connected = false;
    }
}

const Remote::ControlState & Remote::getState() {
    return Remote::_state;
}

bool Remote::isConnected() {
    return Remote::_connected;
}