#ifndef RAYCASTOBJECTMAP_H
#define RAYCASTOBJECTMAP_H

#include "raycast/raycastObject.h"

class raycastObjectMap
{
private:
    uint8_t num_of_objects = 0;
    uint8_t objects_loaded = 0;
    raycastObject* objects = NULL;
public:
    raycastObjectMap(uint8_t obj_count);
    ~raycastObjectMap();
    void push_back(raycastObject object);
    uint8_t size(){return num_of_objects;}
    raycastObject* at(int index){return &objects[index];}
};

#endif //RAYCASTOBJECTMAP_H