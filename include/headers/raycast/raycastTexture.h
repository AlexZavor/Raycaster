#ifndef RAYCASTTEXTURE_H
#define RAYCASTTEXTURE_H

#include "SDL.h"
#include "SDL_image.h"
#include "my_tools/rgba.h"

class raycastTexture{
private:
    SDL_Surface* texture;
public:
    rgba32* pixels;
    rgba32 color;   // Color that the texture replaces
    int w;
    int h;
public:
    raycastTexture(const char* texturePath, rgba32 color){
		texture = IMG_Load(texturePath);
        w = texture->w;
        h = texture->h;
        pixels = (rgba32*)malloc(w*h*sizeof(rgba32));
        for(int i = 0; i < w*h; i++){
		    pixels[i] = rgba32(((uint32_t*)texture->pixels)[i]);
        }
        this->color = color;
    }
    rgba32 getPixel(int x, int y){
        return pixels[x + (y*w)];
    }
};



#endif //RAYCASTTEXTURE_H