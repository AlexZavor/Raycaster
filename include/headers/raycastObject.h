#ifndef RAYCASTOBJECT_H
#define RAYCASTOBJECT_H

#include "rgba.h"
#include "vect2d.h"
#include "SDL_image.h"
#include "SDL.h"

class raycastObject
{
private:
    SDL_Texture* texture;
public:
    vect2d pos;
    rgba32* pixels;
    SDL_Surface* surface;
    int w;
    int h;
public:
    raycastObject(SDL_Renderer* renderer, vect2d position, const char* image);

    SDL_Texture* getTexture(SDL_Renderer* renderer){return texture;}

    void reloadTexture(SDL_Renderer* renderer){texture = SDL_CreateTextureFromSurface(renderer, surface);}
    
};

raycastObject::raycastObject(SDL_Renderer* renderer, vect2d position, const char* image){
    pos = position;
    surface = IMG_Load(image);
    w = surface->w;
    h = surface->h;
    pixels = (rgba32*)surface->pixels;
    reloadTexture(renderer);
}



#endif // RAYCASTOBJECT_H