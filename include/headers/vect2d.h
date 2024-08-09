#ifndef VECT2D_H
#define VECT2D_H

#include <math.h>
#include <random>

#define ZERO_VECT (vect2d(0,0))
#define NORMAL_VECT (vect2d(1,0))

class vect2d {
public:
    float x;
    float y;
public:

    vect2d(float x, float y) {
        this->x = x;
        this->y = y;
    }
    vect2d() {
        this->x = 0;
        this->y = 0;
    }

    float getAngle() {
        return atan(y/x);
    }

    float getMag() {
        return sqrt((x*x) + (y*y));
    }

    vect2d operator+(vect2d const& obj) {
        vect2d value;
        value.x = obj.x + x;
        value.y = obj.y + y;
        return value;
    }

    vect2d operator*(float const& obj) {
        vect2d value;
        value.x = obj * x;
        value.y = obj * y;
        return value;
    }
    
    vect2d operator-(vect2d const& obj) {
        vect2d value;
        value.x = x - obj.x;
        value.y = y - obj.y;
        return value;
    }

    void normalize() {
        float mag = this->getMag();
        x /= mag;
        y /= mag;
    }

    vect2d rotate(float angle) {
        float px = x * cos(angle) - y * sin(angle);
        float py = x * sin(angle) + y * cos(angle);
        x = px;
        y = py;
        return *this;
    }

};

#endif