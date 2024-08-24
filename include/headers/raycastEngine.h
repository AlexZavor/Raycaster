#ifndef RAYCASTENGINE_H
#define RAYCASTENGINE_H

#include "raycast/raycastMap.h"
#include "raycast/raycastObject.h"
#include "raycast/raycastPlayer.h"
#include "raycast/raycastTexture.h"
#include "raycast/raycastRayCalc.h"
#include "framebuffer.h"
#include "sdlTools.h"


void raycastScreen(raycastPlayer player, raycastMap map, framebuffer fbuff, std::vector<raycastTexture>* textureMap = NULL){
    for(int x = 0; x < SCREEN_WIDTH; x++){
        float cameraU = 2 * x / double(SCREEN_WIDTH) - 1;
        vect2d rayDir = player.dir - (player.U*cameraU);

        float dist, xDist;
        rgba32 target;
        bool side;

        calcRay(player, map, rayDir.normal(), &dist, &target, &side, &xDist);

        int drawHeight = (dist*map.worldHeight) - (abs(cameraU)*12);
        if(drawHeight > (SCREEN_HEIGHT/2)-5){drawHeight = (SCREEN_HEIGHT/2)-5;}

        // Find texture in map
        int texture = -1;
        if(textureMap != NULL){
            for(unsigned int i = 0; i < textureMap->size(); i++){
                if(textureMap->at(i).color == target){
                    texture = i;
                    break;
                }
            }
        }

        if(texture != -1){	// Textured drawing
            raycastTexture text = textureMap->at(texture);
            int xpix = xDist*(text).w;
            float pixWidth = (SCREEN_HEIGHT -drawHeight*2)/(float)(text).h; 
            for (int ypix = 0; ypix < (text).h; ypix++){
                fbuff.drawVline(x, drawHeight+(pixWidth*ypix), drawHeight+(pixWidth*(ypix+1)), side ? text.getPixel(xpix,ypix).darken(128) : text.getPixel(xpix,ypix));
            }
        }else{
            fbuff.drawVline(x, drawHeight, SCREEN_HEIGHT-(drawHeight), side ? target.darken(128) : target);
        }
    }
}

void updatePlayer(raycastPlayer* player, input in, raycastMap map){
    if(in.fwrd){player->movePlayer(playerspeed); if(map.mapPoint(player->pos) != rgba32(0)){player->movePlayer(-playerspeed);}}
    if(in.back){player->movePlayer(-playerspeed); if(map.mapPoint(player->pos) != rgba32(0)){player->movePlayer(playerspeed);}}
    if(in.rright){player->rotate(lookspeed);}
    if(in.rleft){player->rotate(-lookspeed);}
}

#endif //RAYCASTENGINE_H