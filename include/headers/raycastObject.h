#ifndef RAYCASTOBJECT_H
#define RAYCASTOBJECT_H

#include "rgba.h"
#include "vect2d.h"
#include "SDL_image.h"

class raycastObject
{
public:
    vect2d pos;
    rgba32* pixels;
    int w;
    int h;
public:
    raycastObject(vect2d position, const char* image);
};

raycastObject::raycastObject(vect2d position, const char* image){
    pos = position;
    SDL_Surface* surface = IMG_Load(image);
    w = surface->w;
    h = surface->h;
    pixels = (rgba32*)surface->pixels;
}



#endif // RAYCASTOBJECT_H