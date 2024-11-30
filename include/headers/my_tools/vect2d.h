#ifndef VECT2D_H
#define VECT2D_H

#include <math.h>

#define ZERO_VECT (vect2d(0,0))
#define NORMAL_VECT (vect2d(1,0))

#define PI_F M_PI

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

    vect2d operator+=(vect2d const& obj) {
        this->x += obj.x;
        this->y += obj.y;
        return *this;
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

    vect2d operator-=(vect2d const& obj) {
        this->x -= obj.x;
        this->y -= obj.y;
        return *this;
    }

    // Applies normalization to vector
    void normalize() {
        float mag = this->getMag();
        x /= mag;
        y /= mag;
    }

    // returns normalized version of vector
    vect2d normal(){
        vect2d value;
        float mag = this->getMag();
        value.x = x/mag;
        value.y = y/mag;
        return value;
    }

    // Rotates vector by angle (Radians)
    vect2d rotate(float angle) {
        float px = x * cos(angle) - y * sin(angle);
        float py = x * sin(angle) + y * cos(angle);
        x = px;
        y = py;
        return *this;
    }

    // returns negative version of vector
    vect2d negative() {
        vect2d value;
        value.x = -x;
        value.y = -y;
        return value;
    }

    // Returns dot product between vector and a, normalized
    float dotProduct(vect2d a){
        a.normalize();
        vect2d b = normal();
        return (a.x*b.x) + (a.y*b.y);
    }

};

#endif