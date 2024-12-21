#ifndef RAYCASTOBJECT_H
#define RAYCASTOBJECT_H

#include "my_tools/rgba.h"
#include "my_tools/vect2d.h"
#include "raycast/raycastPlayer.h"
#include "raycast/raycastMap.h"
#include "raycast/raycastConfig.h"


class raycastObject
{
private:
    SDL_Texture* texture;
public:
    vect2d pos;
    rgba32* pixels;
    rgba32 color;
    SDL_Surface* surface;
    int w;
    int h;
    int scale;
public:
    raycastObject(SDL_Renderer* renderer, vect2d position, const char* image, int scale);

    raycastObject(uint8_t w, uint8_t h, rgba32* pixels, float x, float y, rgba32 color, int scale);

    SDL_Texture* getTexture(SDL_Renderer* renderer){return texture;}

    void reloadTexture(SDL_Renderer* renderer){texture = SDL_CreateTextureFromSurface(renderer, surface);}

    void drawObject(SDL_Renderer* renderer, raycastPlayer player, raycastMap map);
    
};

#endif // RAYCASTOBJECT_H