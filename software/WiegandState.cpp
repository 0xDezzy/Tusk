// vim: ts=4 sw=4 et

#include <Arduino.h>
#include "WiegandState.h"

WiegandState::WiegandState (int id, int basePin)
    : id(id), basePin(basePin), numBitsRead(0), millisLastBitRead(0) {
}

void WiegandState::setup () {
    for (unsigned int pin = 0; pin < 2; ++pin) {
        pinMode(basePin + pin, INPUT_PULLUP);
        prevVals[pin] = digitalRead(basePin + pin);
    }
}

void WiegandState::update () {
    for (unsigned int pin = 0; pin < 2; ++pin) {
        int val = digitalRead(basePin + pin);

        if (val == LOW && prevVals[pin] == HIGH &&
                numBitsRead < ARRAY_SIZE(readBits) * 8) {
            readBits[numBitsRead / 8] <<= 1;
            if (pin == 1)
                readBits[numBitsRead / 8] |= 1;

            ++numBitsRead;
            millisLastBitRead = millis();
        }

        prevVals[pin] = val;
    }
}

bool WiegandState::maybeFlush () {
    bool flushed = false;

    noInterrupts();

    if (millisLastBitRead && millis() > millisLastBitRead + MAX_INTER_BIT_DELAY_MS) {
        if (numBitsRead % 8)
            readBits[numBitsRead / 8] <<= 8 - (numBitsRead % 8);

        Serial.print(id);
        Serial.print(' ');

        Serial.print(numBitsRead);
        Serial.print(' ');

        unsigned int shift = (4 - (numBitsRead % 4)) & 3;
        for (unsigned int i = 0; i < numBitsRead; i += 4) {
            unsigned short v = readBits[i / 8];
            if (i / 8)
                v |= readBits[i / 8 - 1] << 8;
            v >>= 4 - (i % 8) + shift;
            Serial.print(hexChar(v));
        }
        Serial.println();

        numBitsRead = 0;
        millisLastBitRead = 0;

        flushed = true;
    }

    interrupts();

    return flushed;
}
