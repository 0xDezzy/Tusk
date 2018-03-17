// vim: ts=4 sw=4 et

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define ROUND_UP(x, y) (((x) + ((y) - 1)) / (y))

inline char hexChar (unsigned int v) {
    v &= 0xf;
    return v < 10 ? v + '0' : (v - 10) + 'A';
}
