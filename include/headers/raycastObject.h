#ifndef RAYCASTOBJECT_H
#define RAYCASTOBJECT_H

#include "rgba.h"
#include "vect2d.h"
#include "SDL_image.h"
#include "SDL.h"

class raycastObject
{
public:
    vect2d pos;
    rgba32* pixels;
    SDL_Surface* surface;
    int w;
    int h;
public:
    raycastObject(vect2d position, const char* image);

    SDL_Texture* getTexture(SDL_Renderer* renderer){return SDL_CreateTextureFromSurface(renderer, surface);}
    
};

raycastObject::raycastObject(vect2d position, const char* image){
    pos = position;
    surface = IMG_Load(image);
    w = surface->w;
    h = surface->h;
    pixels = (rgba32*)surface->pixels;
}



#endif // RAYCASTOBJECT_H