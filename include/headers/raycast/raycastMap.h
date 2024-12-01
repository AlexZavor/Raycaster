#ifndef RAYCASTMAP_H
#define RAYCASTMAP_H

#include "my_tools/rgba.h"
#include "my_tools/vect2d.h"
#include "my_tools/sdlTools.h"
#include "raycastPlayer.h"
#include "raycast/raycastConfig.h"
#include <vector>


class raycastMap {
private:
    SDL_Surface* surface;
public:
    int colorDepth;
    int w;
    int h;
    rgba32* worldMap;
public:
    raycastMap(){}
    raycastMap( const char* image){
        surface = IMG_Load(image);
        w = surface->w;
        h = surface->h;
        worldMap = (rgba32*)malloc(w*h*sizeof(rgba32));
        std::vector<rgba32> foundColors;
        for(int x = 0; x < w*h; x++){
            rgba32 pixel = rgba32(((uint32_t*)surface->pixels)[x]);
            worldMap[x] = pixel;
            bool found = false;
            for(unsigned int i = 0; i<foundColors.size(); i++){
                if(pixel == foundColors.at(i)){
                    found = true;
                    break;
                }
            }
            if(!found){
                foundColors.push_back(pixel);
            }
        }
        // put found pixels into color map
        colorDepth = foundColors.size();
    }
    raycastMap( int x, int y, char* worldArray, int pallet_size, rgba32* pallet){
        surface = NULL;
        w = x;
        h = y;
        worldMap = (rgba32*)malloc(w*h*sizeof(rgba32));
        for(int x_p = 0; x_p < w*h; x_p++){
            // uint8_t color = pallet[worldArray[x_p]];
            // rgba32 pixel = rgba32(*color, *(color+1), *(color+2), *(color+3));
            worldMap[x_p] = pallet[(int)worldArray[x_p]];
        }
        // put found pixels into color map
        colorDepth = pallet_size;
    }

    void drawMap(SDL_Renderer* renderer, raycastPlayer player){
        // Draw map (if map is texture)
        // SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        // SDL_Rect texr; texr.x = 0; texr.y = 0; 
        //                 texr.w = surface->w*MAP_SCALE; texr.h = surface->h*MAP_SCALE; 
        // SDL_RenderCopy(renderer, texture, NULL, &texr);
        // Old draw method
        for (int x = 0; x < w; x++){
            for (int y = 0; y < h; y++){
                sdlTools_SetRenderDrawColorRGBA(renderer, mapPoint(vect2d(x,y)));
                SDL_Rect rect; rect.x = x*MAP_SCALE; rect.y = y*MAP_SCALE;
                                rect.w = MAP_SCALE; rect.h = MAP_SCALE;
                SDL_RenderFillRect(renderer, &rect);
            }
        }
        // Draw view
        sdlTools_SetRenderDrawColorRGBA(renderer, rgba32(219, 98, 235));
        SDL_Point viewPoints[3]; 
        viewPoints[0].x = (player.pos+player.dir+player.U).x*MAP_SCALE;
        viewPoints[0].y = (player.pos+player.dir+player.U).y*MAP_SCALE;
        viewPoints[1].x = player.pos.x*MAP_SCALE;
        viewPoints[1].y = player.pos.y*MAP_SCALE;
        viewPoints[2].x = (player.pos+player.dir-player.U).x*MAP_SCALE;
        viewPoints[2].y = (player.pos+player.dir-player.U).y*MAP_SCALE;
        SDL_RenderDrawLines(renderer, viewPoints, 3);
        // Draw player
        sdlTools_SetRenderDrawColorRGBA(renderer, rgba32(255, 255, 255));
        SDL_RenderDrawPoint(renderer, player.pos.x*MAP_SCALE, player.pos.y*MAP_SCALE);
    }

    rgba32 mapPoint(vect2d point){
        return worldMap[(int)point.x+(((int)point.y)*w)];
    }
};


#endif //RAYCASTMAP_H