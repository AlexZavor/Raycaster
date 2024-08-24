#ifndef RAYCASTOBJECT_H
#define RAYCASTOBJECT_H

#include "rgba.h"
#include "vect2d.h"
#include "SDL_image.h"
#include "SDL.h"
#include "globals.h"
#include "raycast/raycastRayCalc.h"

#define OBJ_DEFAULT_SCALE 20

class raycastObject
{
private:
    SDL_Texture* texture;
public:
    vect2d pos;
    rgba32* pixels;
    SDL_Surface* surface;
    int w;
    int h;
    int scale;
public:
    raycastObject(SDL_Renderer* renderer, vect2d position, const char* image, int scale);

    SDL_Texture* getTexture(SDL_Renderer* renderer){return texture;}

    void reloadTexture(SDL_Renderer* renderer){texture = SDL_CreateTextureFromSurface(renderer, surface);}

    void drawObject(SDL_Renderer* renderer, raycastPlayer player, raycastMap map);
    
};

raycastObject::raycastObject(SDL_Renderer* renderer, vect2d position, const char* image, int scale = OBJ_DEFAULT_SCALE){
    pos = position;
    this->scale = scale;
    surface = IMG_Load(image);
    w = surface->w;
    h = surface->h;
    pixels = (rgba32*)surface->pixels;
    reloadTexture(renderer);
}

void raycastObject::drawObject(SDL_Renderer* renderer, raycastPlayer player, raycastMap map) {
	vect2d objectDir = (player.pos-pos); 
	vect2d weirdPlayer = player.dir;
	weirdPlayer.rotate(PI_F/2);	// Makes ratio of dot product more useful for mapping across screen

	float ang = weirdPlayer.dotProduct(objectDir);
	float view = weirdPlayer.normal().dotProduct((player.dir+player.U).normal());

	if(ang > view && ang < -view && player.dir.dotProduct(objectDir)<.5){// Check if in view
		float dist = (objectDir).getMag();
		float walldist; rgba32 tar; bool side;
		calcRay(player,map,objectDir.normal().negative(),&walldist, &tar, &side);

		if(dist<walldist){										// Check if not behind wall
			int drawX = -(ang * (SCREEN_WIDTH) / (abs(view*2))) + SCREEN_WIDTH/2 - (w/2);
			int drawHeight = (dist*map.worldHeight) +h*scale/dist;
			if(drawHeight > (SCREEN_HEIGHT/2)+(h/2)){drawHeight = (SCREEN_HEIGHT/2)+(h/2);}

			SDL_Rect texr; texr.x = drawX; texr.y = SCREEN_HEIGHT - drawHeight; 
						   texr.w = w*scale/dist; texr.h = h*scale/dist; 
			SDL_RenderCopy(renderer, getTexture(renderer), NULL, &texr);
		}
	}
}

#endif // RAYCASTOBJECT_H