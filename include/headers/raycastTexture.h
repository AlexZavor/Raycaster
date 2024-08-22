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
    rgba32 color;
    int w;
    int h;
public:
    raycastTexture(const char* texturePath, rgba32 color){
		texture = IMG_Load(texturePath);
		pixels = (rgba32*)texture->pixels;
        w = texture->w;
        h = texture->h;
        this->color = color;
    }
    rgba32 getPixel(int x, int y){
        return pixels[x + (y*w)];
    }
};



#endif //RAYCASTTEXTURE_H