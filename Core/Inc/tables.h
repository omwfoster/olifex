#ifndef TABLES_H
#define TABLES_H


extern const uint8_t sine[17];
#define DIM(c)     (uint32_t)((c >> 2) & 0x3f3f3f3f) // color at 25% intensity
#define DARK(c)    (uint32_t)((c >> 4) & 0x0f0f0f0f) // color at  6% intensity

extern const uint8_t colorsFull[766][3];


#endif
