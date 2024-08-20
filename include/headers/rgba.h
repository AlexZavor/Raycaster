#ifndef RGBA_H
#define RGBA_H

#include <stdint.h>
#include "SDL.h"

class rgba32 {
public:
    uint8_t b;
    uint8_t g;
    uint8_t r;
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
    rgba32 darken(uint8_t value){
        float amount = value/255.0f;
        uint8_t newR = r*amount;
        uint8_t newG = g*amount;
        uint8_t newB = b*amount;
        return rgba32(newR, newG, newB, a);
    }
    bool operator==(rgba32 const& obj) {
        return ((obj.r == r) && (obj.g == g) && (obj.b == b) && (obj.a == a));
    }
};


void RGBA_SetRenderDrawColor(SDL_Renderer* renderer, rgba32 color, bool shadow = false){
    if(shadow){
        color = color.darken(128);
    }
    SDL_SetRenderDrawColor(renderer, color.r,color.g,color.b,color.a);
}


#endif //RGBA_H