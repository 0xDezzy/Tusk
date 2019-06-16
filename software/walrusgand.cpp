// vim: ts=4 sw=4 et

#include <Arduino.h>
#include "WiegandState.h"

const unsigned int SERIAL_BAUD_RATE = 9600;

const unsigned long LED_CYCLE_MS = 10000, LED_ON_MS = 50;

WiegandState wiegandStates[3] = {
    WiegandState(1, 2),
    WiegandState(2, 4),
    WiegandState(3, 6)
};

void setup () {
    Serial.begin(SERIAL_BAUD_RATE);

    pinMode(LED_BUILTIN, OUTPUT);

    for (unsigned int i = 0; i < ARRAY_SIZE(wiegandStates); ++i)
        wiegandStates[i].setup();

    PCMSK2 = bit(PCINT18) | bit(PCINT19) | bit(PCINT20) | bit(PCINT21) |
             bit(PCINT22) | bit(PCINT23);
    PCIFR |= bit(PCIF2);
    PCICR |= bit(PCIE2);
}

void loop () {
    static size_t blinks = 0;
    static unsigned long nextBlinkMillis;

    for (unsigned int i = 0; i < ARRAY_SIZE(wiegandStates); ++i)
        if (wiegandStates[i].maybeFlush()) {
            blinks = wiegandStates[i].getID() * 2 + 1;
            nextBlinkMillis = millis() + 300;
        }

    if (blinks && millis() >= nextBlinkMillis) {
        --blinks;
        nextBlinkMillis = millis() + 300;
    }

    digitalWrite(LED_BUILTIN,
            (blinks ?
              !(blinks % 2) :
              millis() % LED_CYCLE_MS < LED_ON_MS) ? HIGH : LOW);
}

ISR (PCINT2_vect) {
    for (unsigned int i = 0; i < ARRAY_SIZE(wiegandStates); ++i)
        wiegandStates[i].update();
}
