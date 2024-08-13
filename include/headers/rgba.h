#ifndef RGBA_H
#define RGBA_H

#include <stdint.h>

class rgba32
{
public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    rgba32(uint32_t color){
        r = color              >> 24;
        g = (color&0x00FF0000) >> 16;
        b = (color&0x0000FF00) >> 8;
        a = (color&0x000000FF);
    }
    rgba32(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255){
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
};



#endif //RGBA_H