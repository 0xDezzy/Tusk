// vim: ts=4 sw=4 et
//
#ifndef WALRUSGAND_WIEGANDSTATE_H
#define WALRUSGAND_WIEGANDSTATE_H

#include <stddef.h>
#include "util.h"

class WiegandState {
    static const int MAX_INTER_BIT_DELAY_MS = 250;

    const int id, basePin;

    int prevVals[2];

    unsigned char readBits[ROUND_UP(256, 8)];
    size_t numBitsRead;
    unsigned long millisLastBitRead;

public:
    WiegandState (int id, int basePin);

    void setup ();
    void update ();
    bool maybeFlush ();

    inline int getID () const {
        return id;
    }
};

#endif
