#ifndef RAYCASTPLAYER_H
#define RAYCASTPLAYER_H

#include "my_tools/vect2d.h"

class raycastPlayer
{
private:
public:
    vect2d pos;
    vect2d dir;
    vect2d U; // Sets fov, scale dependant
public:
    raycastPlayer(vect2d pos, vect2d dir = vect2d(-1,0), vect2d U = vect2d(0,0.50)){
        this->pos = pos;
        this->dir = dir;
        this->U = U;
    }
    raycastPlayer(){};
    void rotate(float angle){
        dir.rotate(angle); U.rotate(angle);
    }
    void movePlayer(float movement){
        pos+=(dir*movement);
    }
};




#endif // RAYCASTPLAYER_H