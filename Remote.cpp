#include <Arduino.h>
#include "Remote.h"

//#define SOFTSERIAL

unsigned long Remote::_lastSeen = 0;
bool Remote::_connected = false;
Remote::ControlState Remote::_state = {0, 0, 0, false};

#ifdef SOFTSERIAL
#include <SoftwareSerial.h>
SoftwareSerial softSerial(11, 13);
#endif // SOFTSERIAL

Remote::ControlState data;
void Remote::update() {
#ifdef SOFTSERIAL
    if (softSerial.available() >= sizeof(ControlState) + 1) {
        softSerial.readBytes((char*)&data, sizeof(ControlState));
        softSerial.find('\n');
#else
    if (Serial.available() >= sizeof(ControlState) + 1) {
        Serial.readBytes((char*)&data, sizeof(ControlState));
        Serial.find('\n');
#endif // SOFTSERIAL

        if(data.power <= 512 && data.steering <= 1024 && data.steering >= -1024 && (data.direction == 1 || data.direction == 0 || data.direction == -1)) {
            _connected = true;
            Remote::_state = data;
            _lastSeen = millis();
        }
    }

    if(millis() - _lastSeen > _timeout && _connected) {
        _connected = false;
        reset();
    }
}

const Remote::ControlState & Remote::getState() {
    return Remote::_state;
}

bool Remote::isConnected() {
    return Remote::_connected;
}

void Remote::init() {
    reset();
#ifdef SOFTSERIAL
    softSerial.write("$$$");
    softSerial.write("R,1");
    softSerial.end();
    softSerial.begin(115200);
#else
    Serial.begin(115200);
    Serial.print("$");
    Serial.print("$");
    Serial.print("$");
    delay(100);
    Serial.println("SQ,128");
    delay(100);
    Serial.println("---");
#endif
}

void Remote::reset() {
    #ifdef SOFTSERIAL
    softSerial.write("$$$");
    softSerial.write("R,1");
    softSerial.end();
    softSerial.begin(115200);
#else
    Serial.begin(115200);
    Serial.print("$");
    Serial.print("$");
    Serial.print("$");
    delay(100);
    Serial.println("R,1");
    delay(100);
    Serial.println("---");
#endif
}