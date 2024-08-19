#ifndef RAYCASTMAP_H
#define RAYCASTMAP_H

// #include <vector>
// #include <algorithm>
#include "SDL.h"
#include "SDL_image.h"
#include "rgba.h"
#include "vect2d.h"
#include "raycastPlayer.h"

class raycastMap {
private:
    SDL_Surface* surface;
public:
    int colorDepth;
    int w;
    int h;
    int* worldMap;
    rgba32* colorMap;
public:
    raycastMap( const char* image){
        surface = IMG_Load(image);
        w = surface->w;
        h = surface->h;
        worldMap = (int*)malloc(w*h*sizeof(int));
        std::vector<rgba32> foundColors;
        foundColors.push_back(rgba32(0,0,0));// color 0 is always black
        for(int x = 0; x < w*h; x++){
            rgba32 pixel = (((rgba32*)surface->pixels)[x]);
            bool found = false;
            for(unsigned int i = 0; i<foundColors.size(); i++){
                if(pixel == foundColors.at(i)){
                    worldMap[x] = (int)i;
                    found = true;
                    break;
                }
            }
            if(!found){
                foundColors.push_back(pixel);
                worldMap[x] = foundColors.size();
            }
        }
        // put found pixels into color map
        colorDepth = foundColors.size()+1;
        colorMap = (rgba32*)malloc(foundColors.size()*sizeof(rgba32));
        for(int x = foundColors.size(); x >= 0; x--){
            colorMap[x] = foundColors.back();
            foundColors.pop_back();
        }
        // printf("size of vector after - %d\n", foundColors.size());
    }

    rgba32 getColor(int color, bool shadow = false){
        if(color>colorDepth){	// Error
            return rgba32(255, 0, 189);
        }
        if(shadow){
            return colorMap[color].darken(128);
        }
        return colorMap[color];
    }

    void drawMap(SDL_Renderer* renderer, raycastPlayer player){
        int MMScale = 4;
        // Draw map
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect texr; texr.x = 0; texr.y = 0; 
                        texr.w = surface->w*MMScale; texr.h = surface->h*MMScale; 
        SDL_RenderCopy(renderer, texture, NULL, &texr);
        // Old draw method
        /*for (int x = 0; x < w; x++){
            for (int y = 0; y < h; y++){
                RGBA_SetRenderDrawColor(renderer, getColor(mapPoint(vect2d(x,y))));
                SDL_Rect rect; rect.x = x*MMScale; rect.y = y*MMScale;
                                rect.w = MMScale; rect.h = MMScale;
                SDL_RenderFillRect(renderer, &rect);
            }
        }*/
        // Draw view
        RGBA_SetRenderDrawColor(renderer, getColor(9));
        SDL_Point viewPoints[3]; 
        viewPoints[0].x = (player.pos+player.dir+player.U).x*MMScale;
        viewPoints[0].y = (player.pos+player.dir+player.U).y*MMScale;
        viewPoints[1].x = player.pos.x*MMScale;
        viewPoints[1].y = player.pos.y*MMScale;
        viewPoints[2].x = (player.pos+player.dir-player.U).x*MMScale;
        viewPoints[2].y = (player.pos+player.dir-player.U).y*MMScale;
        SDL_RenderDrawLines(renderer, viewPoints, 3);
        // Draw player
        RGBA_SetRenderDrawColor(renderer, getColor(5));
        SDL_RenderDrawPoint(renderer, player.pos.x*MMScale, player.pos.y*MMScale);
    }

    int mapPoint(vect2d point){
        return worldMap[(int)point.x+((int)point.y*w)];
    }
};


#endif //RAYCASTMAP_H