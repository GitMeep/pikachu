#pragma once

/*
* Denne klasses opgave er at modtage opdateringer fra fjernbetjeningen og holde en intern tilstand
* som gerne skulle være synkroniseret med fjernbetjeningen. Der holdes også styr på hvornår en 
* valid opdatering sidst blev modtaget, som bliver brugt til at bestemme hvorvidt fjernbetjeningen
* stadig er forbundet.
*
* Dataen modtages via hardware-serielforbindelsen. Denne kan forbindes med kobber, eller et
* transparent bluetooth/radio link kan bruges.
*/
class Remote {
public:
    // datastruktur der beskriver en besked fra fjernbetjeningen
    struct ControlState {
        unsigned long power; // motor power (mellem 0 og 511)
        long steering; // retning hvori der skal styres (mellem -1024 for venstre og +1024 for højre, 0 ligeud)
        char direction; // retning som skal køres (frem: 1, tilbage: -1, stille: 0)
        bool fire; // hvorvidt kannonen skal køre
    };

    static void update(); // funktion som skal køres hvert loop og opdaterer tilstanden
    static const ControlState & getState(); // få tilstanden
    static bool isConnected(); // er fjernbetjeningen forbundet?

    static void init(); // set indstillinger på modul
    static void reset(); // reboot modul (virker ikke)

private:
    // timeout
    unsigned const static int _timeout = 500; // tid i ms som skal gå fra sidste modtagede besked til at forbindelsen timer ud
    unsigned static long _lastSeen; // tid i ms siden start hvor sidste besked blev modtaget
    static bool _connected; // tror vi at forbindelsen er oppe?

    static ControlState _state; // tilstanden af fjernbetjeningen
};