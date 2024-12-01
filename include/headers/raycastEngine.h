#ifndef RAYCASTENGINE_H
#define RAYCASTENGINE_H

#include "raycast/raycastMap.h"
#include "raycast/raycastObject.h"
#include "raycast/raycastPlayer.h"
#include "raycast/raycastTexture.h"
#include "raycast/raycastRayCalc.h"
#include "raycast/raycastFileLoader.h"
#include "raycast/raycastConfig.h"
#include "my_tools/framebuffer.h"
#include "my_tools/sdlTools.h"


void raycastScreen(raycastPlayer player, raycastMap* map, framebuffer* fbuff, std::vector<raycastTexture>* textureMap = NULL){
    static int s_height = fbuff->screenHeight;
    for(int x = 0; x < fbuff->screenWidth; x++){
        float cameraU = ((2 * x) / ((float)fbuff->screenWidth))-1;
        vect2d rayDir = player.dir - (player.U*cameraU);

        float dist, xDist;
        rgba32 target;
        bool side;

        calcRay(player, map, rayDir.normal(), &dist, &target, &side, &xDist);

        int drawHeight = (dist*WORLD_HEIGHT) - 80;
        if(drawHeight > (s_height/2)-5){drawHeight = (s_height/2)-5;}

        // Find texture in map
        int texture = -1;
        if(textureMap != NULL ){
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
            float pixWidth = (s_height -drawHeight*2)/(float)(text).h; 
            for (int ypix = 0; ypix < (text).h; ypix++){
                fbuff->drawVline(x, drawHeight+(pixWidth*ypix), drawHeight+(pixWidth*(ypix+1)), side ? text.getPixel(xpix,ypix).darken(128) : text.getPixel(xpix,ypix));
            }
        }else{
            fbuff->drawVline(x, drawHeight, s_height-(drawHeight), side ? target.darken(128) : target);
        }
    }
}

void updatePlayer(raycastPlayer* player, input in, raycastMap map){
    if(in.fwrd){player->movePlayer(PLAYER_SPEED); if(map.mapPoint(player->pos) != rgba32(0)){player->movePlayer(-PLAYER_SPEED);}}
    if(in.back){player->movePlayer(-PLAYER_SPEED); if(map.mapPoint(player->pos) != rgba32(0)){player->movePlayer(PLAYER_SPEED);}}
    if(in.rright){player->rotate(LOOK_SPEED);}
    if(in.rleft){player->rotate(-LOOK_SPEED);}
}

#endif //RAYCASTENGINE_H