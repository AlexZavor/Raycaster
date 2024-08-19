#ifndef RAYCASTTEXTURE_H
#define RAYCASTTEXTURE_H

#include "SDL.h"
#include "SDL_image.h"
#include "rgba.h"

class raycastTexture{
private:
    SDL_Surface* texture;
public:
    rgba32* pixels;
    int w;
    int h;
public:
    raycastTexture(const char* texturePath){
		texture = IMG_Load(texturePath);
		pixels = (rgba32*)texture->pixels;
        w = texture->w;
        h = texture->h;
    }
    rgba32 getPixel(int x, int y){
        return pixels[x + (y*w)];
    }
};



#endif //RAYCASTTEXTURE_H