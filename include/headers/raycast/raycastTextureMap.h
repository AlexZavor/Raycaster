#ifndef RAYCASTTEXTUREMAP_H
#define RAYCASTTEXTUREMAP_H

#include "raycast/raycastTexture.h"

class raycastTextureMap
{
private:
    uint8_t num_of_textures = 0;
    uint8_t textures_loaded = 0;
    raycastTexture* textures = NULL;
public:
    raycastTextureMap(uint8_t texture_count);
    ~raycastTextureMap();
    void push_back(raycastTexture texture);
    uint8_t size(){return num_of_textures;}
    raycastTexture* at(int index){return &textures[index];}
};

#endif //RAYCASTTEXTUREMAP_H