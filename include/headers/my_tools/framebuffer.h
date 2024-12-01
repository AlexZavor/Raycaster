#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#pragma once

// Credit to DOOMReboot on Github for the pixel pusher system! https://github.com/DOOMReboot

#include "SDL.h"
#include "my_tools/rgba.h"
#include <stdlib.h>

class framebuffer
{
private:
    SDL_Texture* texture;
    int32_t pitch;
    uint32_t* pixelBuffer;
public:
    int screenWidth;
    int screenHeight;
    rgba32* frameBuffer;
public:
    // Construct Frame buffer
    framebuffer(SDL_Texture* texture, int screenWidth, int screenHeight);
    bool pushFrame(SDL_Renderer* renderer);
    // Fast draw vertical line
    void drawVline(int x, int y1, int y2, rgba32 color);
    // Deconstruct Frame buffer
    ~framebuffer();
};

#endif //FRAMEBUFFER_H
