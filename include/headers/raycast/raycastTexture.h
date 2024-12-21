#ifndef RAYCASTTEXTURE_H
#define RAYCASTTEXTURE_H

#include "SDL.h"
#include "SDL_image.h"
#include "my_tools/rgba.h"

class raycastTexture{
private:
    SDL_Surface* texture;
    rgba32* pixels;
public:
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
    raycastTexture(int t_w, int t_h, rgba32* t_pixels, rgba32 repl_color){
        w = t_w;
        h = t_h;
        pixels = (rgba32*)malloc(w*h*sizeof(rgba32));
        SDL_memcpy(pixels, t_pixels, w*h*sizeof(rgba32));
        color = repl_color;
    }
    rgba32 getPixel(int x, int y){
        return pixels[x + (y*w)];
    }
    ~raycastTexture(){
        // Let freedom... not ring
        // free(pixels);
    }
};



#endif //RAYCASTTEXTURE_H