#ifndef RGBA_H
#define RGBA_H
#pragma once

#include <stdint.h>

class rgba32 {
public:
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;

    rgba32(){r=0;g=0;b=0;a=255;}
    rgba32(uint32_t color){
        a = color              >> 24;
        b = (color&0x00FF0000) >> 16;
        g = (color&0x0000FF00) >> 8;
        r = (color&0x000000FF);
    }
    rgba32(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255){
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
    // Darken the color by value/255
    rgba32 darken(uint8_t value){
        float amount = value/255.0f;
        // uint8_t newR = r*amount;
        // uint8_t newG = g*amount;
        // uint8_t newB = b*amount;
        return rgba32(r*amount, g*amount, b*amount, a);
    }
    bool operator==(rgba32 const& obj) {
        return ((obj.r == r) && (obj.g == g) && (obj.b == b));
    }
    bool operator!=(rgba32 const& obj) {
        return ((obj.r != r) || (obj.g != g) || (obj.b != b));
    }
};



#endif //RGBA_H