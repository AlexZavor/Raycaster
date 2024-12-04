#ifndef SDLTOOLS_H
#define SDLTOOLS_H
#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "my_tools/rgba.h"

//Input map
typedef struct{
    bool quit = 0;
    bool fwrd = 0;
    bool back = 0;
    bool rleft = 0;
    bool rright = 0;
    bool sel = 0;
} input;


bool initializeSDL(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture, int width, int height);

bool closeSDL(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture);

void sdlTools_handleInput(SDL_Event e, input* in);

void sdlTools_SetRenderDrawColorRGBA(SDL_Renderer* renderer, rgba32 color, bool shadow = false);

#endif //SDLTOOLS_H