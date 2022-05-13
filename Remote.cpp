#include <Arduino.h>
#include "Remote.h"

// preprocessor makro der definerer om der skal bruges software eller hardware serial
//#define SOFTSERIAL

// alloker og initialiser statiske medlemmer
unsigned long Remote::_lastSeen = 0;
bool Remote::_connected = false;
Remote::ControlState Remote::_state = {0, 0, 0, false};

// inkluder og konstruer software serial hvis makroen er defineret
#ifdef SOFTSERIAL
#include <SoftwareSerial.h>
SoftwareSerial softSerial(11, 13);
#endif // SOFTSERIAL

// funktion der opdaterer den interne tilstand af fjernbetjeningen hvis der er en opdatering
Remote::ControlState data;
void Remote::update() {
    // læs fjernbetjening-tilstand ind fra serial hvis der er nok data tilgængeligt (\n angiver slut på en transmission)
#ifdef SOFTSERIAL
    if (softSerial.available() >= sizeof(ControlState) + 1) {
        softSerial.readBytes((char*)&data, sizeof(ControlState));
        softSerial.find('\n');
#else
    if (Serial.available() >= sizeof(ControlState) + 1) {
        Serial.readBytes((char*)&data, sizeof(ControlState));
        Serial.find('\n');
#endif // SOFTSERIAL

        // check om data er intakt (værdier er indenfor forventede grænser), kopier opdatering til intern tilstand og nulstil timeout timer
        if(data.power <= 512 && data.steering <= 1024 && data.steering >= -1024 && (data.direction == 1 || data.direction == 0 || data.direction == -1)) {
            _connected = true;
            Remote::_state = data;
            _lastSeen = millis();
        }
    }

    // marker at forbindelsen er mistet hvis en besked ikke er blevet modtaget i noget tid
    if(millis() - _lastSeen > _timeout && _connected) {
        _connected = false;
        reset(); // reboot bluetooth modulet (virker ikke)
    }
}

// få den interne tilstand
const Remote::ControlState & Remote::getState() {
    return Remote::_state;
}

// er fjernbetjeningen forbundet?
bool Remote::isConnected() {
    return Remote::_connected;
}

void Remote::init() {
    reset();
}

// skulle gerne reboot'e bluetooth modulet, men det virker ikke
void Remote::reset() {
    #ifdef SOFTSERIAL
    softSerial.begin(115200);
    softSerial.print("$");
    softSerial.print("$");
    softSerial.print("$");
    delay(100);
    softSerial.println("R,1");
    delay(100);
    softSerial.println("---");
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