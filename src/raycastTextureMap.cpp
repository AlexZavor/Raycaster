#include "raycast/raycastTextureMap.h"
#include <stdio.h>

raycastTextureMap::raycastTextureMap(uint8_t texture_count){
    num_of_textures = texture_count;
    textures_loaded = 0;
    textures = (raycastTexture*)malloc(texture_count*sizeof(raycastTexture));
}

raycastTextureMap::~raycastTextureMap()
{
    for(int n = 0; n < num_of_textures; n++){
        delete &textures[n];
    }
    free(textures);
}

void raycastTextureMap::push_back(raycastTexture texture){
    // SDL_memcpy(textures+(textures_loaded*sizeof(raycastTexture)), &texture, sizeof(raycastTexture));
    textures[textures_loaded] = texture;
    textures_loaded++;
}