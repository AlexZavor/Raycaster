#ifndef RAYCASTRAYCALC_H
#define RAYCASTRAYCALC_H

#include "raycast/raycastMap.h"
#include "raycast/raycastPlayer.h"

static void calcRay(raycastPlayer player, raycastMap* map, 
			 vect2d rayDir, float* dist, rgba32* target, bool* side, float* xDist = NULL){
	vect2d mapTile = vect2d((int)player.pos.x, (int)player.pos.y); //truncate to get start tile. 

	// float slopeY = (rayDir.y*rayDir.y)/(rayDir.x*rayDir.x);
	// float deltaX = sqrt(1+(1/slopeY));
	// float deltaY = sqrt(1+slopeY);
	double deltaX = (rayDir.x == 0) ? 1e30 : std::abs(1 / rayDir.x);
	double deltaY = (rayDir.y == 0) ? 1e30 : std::abs(1 / rayDir.y);

	int stepX;
	int stepY;
	float nextX;
	float nextY;
	
	if (rayDir.x < 0) {	// ray on left
		stepX = -1;
		nextX = (player.pos.x - mapTile.x) * deltaX;
	} else {			// ray on right
		stepX = 1;
		nextX = (mapTile.x + 1.0 - player.pos.x) * deltaX;
	} if (rayDir.y < 0) {	// ray down
		stepY = -1;
		nextY = (player.pos.y - mapTile.y) * deltaY;
	} else {				// ray up
		stepY = 1;
		nextY = (mapTile.y + 1.0 - player.pos.y) * deltaY;
	}

	// DDA raycast
	while (map->mapPoint(mapTile) == rgba32(0)){
		if(nextX < nextY){  // hits new X tile
			*dist = nextX;
			*side = false;
			nextX += deltaX;
			mapTile.x += stepX;
		} else {			// hits new Y tile
			*dist = nextY;
			*side = true;
			nextY += deltaY;
			mapTile.y += stepY;
		}
	}

	if(xDist != NULL){
		vect2d newPoint = (player.pos + (rayDir*(*dist)));
		*xDist = *side ? newPoint.x-(int)newPoint.x : newPoint.y-(int)newPoint.y;
	}

	*target = map->mapPoint(mapTile);
	return;
}

#endif //RAYCASTRAYCALC_H