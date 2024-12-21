#include "raycast/raycastObjectMap.h"
#include <stdio.h>

raycastObjectMap::raycastObjectMap(uint8_t texture_count){
    num_of_objects = texture_count;
    objects_loaded = 0;
    objects = (raycastObject*)malloc(texture_count*sizeof(raycastObject));
}

raycastObjectMap::~raycastObjectMap()
{
    for(int n = 0; n < num_of_objects; n++){
        delete &objects[n];
    }
    free(objects);
}

void raycastObjectMap::push_back(raycastObject texture){
    // SDL_memcpy(textures+(textures_loaded*sizeof(raycastTexture)), &texture, sizeof(raycastTexture));
    objects[objects_loaded] = texture;
    objects_loaded++;
}