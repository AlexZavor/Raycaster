#include <stdio.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "raycastObject.h"
#include "raycastPlayer.h"
#include "math.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define WORLD_HEIGHT 20

#define PI_HALF 1.57079632679


#define MAP_WIDTH 24
#define MAP_HEIGHT 24

#define FLOWER_PATH "Res/Flower.png"
#define WALL_TEXUTRE_PATH "Res/WallTexture.png"

//Input map
bool fwrd = 0;
bool back = 0;
bool rleft = 0;
bool rright = 0;
float playerspeed = 0.05;
float lookspeed = 0.02;

int worldMap[MAP_WIDTH][MAP_HEIGHT]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};


bool initializeSDL(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture){
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return 0;
	}
	//Create window
	*window = SDL_CreateWindow( "RayCaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if( *window == NULL ) {
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return 0;
	}
	//Create vsynced renderer for window
	*renderer = SDL_CreateRenderer( *window, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/ );
	if( *renderer == NULL ) {
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
	}
	//Create texture to render pixels to.
	*texture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	if( *texture == NULL ) {
		printf( "Texture could not be created! SDL Error: %s\n", SDL_GetError() );
	}

	// SDL_RenderSetScale(*renderer, SCALE, SCALE);

	TTF_Init();
	IMG_Init(IMG_INIT_PNG);

	return true;
}

bool closeSDL(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture){
	//Destroy window
	if( texture != NULL ) {
		SDL_DestroyTexture( texture );
	}
	//Destroy renderer
	if( renderer != NULL ) {
		SDL_DestroyRenderer(renderer);
	}
	//Destroy window
	if( window != NULL ) {
		SDL_DestroyWindow( window );
	}
	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return true;
}


void calcRay(raycastPlayer player, vect2d rayDir, float* dist, int* target, bool* side, float* xDist = nullptr){
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
	while (worldMap[(int)mapTile.x][(int)mapTile.y] == 0){
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

	if(xDist != nullptr){
		vect2d newPoint = (player.pos + (rayDir*(*dist)));
		*xDist = *side ? newPoint.x-(int)newPoint.x : newPoint.y-(int)newPoint.y;
	}

	*target = worldMap[(int)mapTile.x][(int)mapTile.y];
	return;
}

void setRenderColor(SDL_Renderer* renderer, int color, bool side = false){
	switch (color)
	{
	case 0:
		SDL_SetRenderDrawColor(renderer, 0,0,0,255);
		break;
	case 1:
		SDL_SetRenderDrawColor(renderer, 0,0,255-(side*100),255);
		break;
	case 2:
		SDL_SetRenderDrawColor(renderer, 255-(side*100),0,0,255);
		break;
	case 3:
		SDL_SetRenderDrawColor(renderer, 0,255-(side*100),0,255);
		break;
	case 4:
		SDL_SetRenderDrawColor(renderer, 0,255-(side*100),255-(side*100),255);
		break;
	case 5:
		SDL_SetRenderDrawColor(renderer, 227, 225, 90,255);
		break;
	
	default:
		SDL_SetRenderDrawColor(renderer, 255,45,229,255);
		break;
	}
}

void setRenderColor(SDL_Renderer* renderer, rgba32 color, bool side = false){
	if(side){
		color = color.darken(128);
	}
	SDL_SetRenderDrawColor(renderer, color.r,color.g,color.b,color.a);
}

void drawObject(SDL_Renderer* renderer, raycastPlayer player, raycastObject object) {
	vect2d objectDir = (player.pos-object.pos); 
	vect2d weirdPlayer = player.dir;
	weirdPlayer.rotate(PI_HALF);	// Makes ratio of dot product more useful for mapping across screen

	float ang = weirdPlayer.dotProduct(objectDir);
	float view = weirdPlayer.normal().dotProduct((player.dir+player.U).normal());

	if(ang > view && ang < -view && player.dir.dotProduct(objectDir)<.5){// Check if in view
		float dist = (objectDir).getMag();
		float walldist; int tar; bool side;
		calcRay(player,objectDir.normal().negative(),&walldist, &tar, &side);

		if(dist<walldist){										// Check if not behind wall
			int drawX = -(ang * (SCREEN_WIDTH) / (abs(view*2))) + SCREEN_WIDTH/2 - (object.w/2);
			int drawHeight = (dist*WORLD_HEIGHT) +object.h*20/dist;
			if(drawHeight > (SCREEN_HEIGHT/2)+(object.h/2)){drawHeight = (SCREEN_HEIGHT/2)+(object.h/2);}

			SDL_Rect texr; texr.x = drawX; texr.y = SCREEN_HEIGHT - drawHeight; 
						   texr.w = object.w*20/dist; texr.h = object.h*20/dist; 
			SDL_RenderCopy(renderer, object.getTexture(renderer), NULL, &texr);
		}
	}
}

int mapPoint(vect2d point){
	return worldMap[(int)point.x][(int)point.y];
}

int main(int argc, char* argv[]) {
	// The window we'll be rendering to
	SDL_Window* window = NULL;
	// The window renderer
	SDL_Renderer* renderer = NULL;
	// The texture to render to
    SDL_Texture* texture = NULL;
	// rect for clearing quickly
	// Event handler
	SDL_Event e;

	if(initializeSDL(&window, &renderer, &texture)){
		// Main loop logic
		bool quit = false;

		// Game Objects

		raycastPlayer player = raycastPlayer(vect2d(12,12));
		raycastObject flower = raycastObject(vect2d(3,3), FLOWER_PATH);

		// Textures
		SDL_Surface* wall = IMG_Load(WALL_TEXUTRE_PATH);
		rgba32* wallArray = (rgba32*)wall->pixels;
		// printf("%i\n", wallArray[12].r);

		while(!quit){
			Uint64 start = SDL_GetPerformanceCounter();


			//Handle events on queue
			while( SDL_PollEvent( &e ) != 0 ) {
				//User requests quit (X button)
				if( e.type == SDL_QUIT ) {
					quit = true;
				}
				//User presses a key
				else if( e.type == SDL_KEYDOWN ) {
					switch(e.key.keysym.sym){
						case SDLK_ESCAPE:
							quit = true;
						break;
						case SDLK_w:
							fwrd = true;
						break;
						case SDLK_s:
							back = true;
						break;
						case SDLK_a:
							rleft = true;
						break;
						case SDLK_d:
							rright = true;
						break;
						default:
						break;
					}
				}
				else if( e.type == SDL_KEYUP ) {
					switch(e.key.keysym.sym){
						case SDLK_w:
							fwrd = false;
						break;
						case SDLK_s:
							back = false;
						break;
						case SDLK_a:
							rleft = false;
						break;
						case SDLK_d:
							rright = false;
						break;
						default:
						break;
					}
				}
			}

			//update player
			if(fwrd){player.movePlayer(playerspeed); if(mapPoint(player.pos) != 0){player.movePlayer(-playerspeed);}}
			if(back){player.movePlayer(-playerspeed); if(mapPoint(player.pos) != 0){player.movePlayer(playerspeed);}}
			if(rright){player.rotate(lookspeed);}
			if(rleft){player.rotate(-lookspeed);}
			
			//Update screen
			for(int x = 0; x < SCREEN_WIDTH; x++){
				float cameraU = 2 * x / double(SCREEN_WIDTH) - 1;
				vect2d rayDir = player.dir - (player.U*cameraU);

				float dist, xDist;
				int target;
				bool side;

				calcRay(player, rayDir.normal(), &dist, &target, &side, &xDist);

				if(target == 1){	// Textured drawing
					int xpix = xDist*wall->w;
					int drawHeight = (dist*WORLD_HEIGHT) - (abs(cameraU)*12);
					if(drawHeight > (SCREEN_HEIGHT/2)-5){drawHeight = (SCREEN_HEIGHT/2)-5;}
					float pixWidth = (SCREEN_HEIGHT -drawHeight*2)/(float)wall->h; 
					for (int ypix = 0; ypix < wall->h; ypix++){
						setRenderColor(renderer, *(wallArray+(xpix+(ypix*wall->w))), side);
						SDL_RenderDrawLine(renderer, x, drawHeight+(pixWidth*ypix), x, drawHeight+(pixWidth*(ypix+1)));
					}
				}else{
					setRenderColor(renderer, target, side);
					int drawHeight = (dist*WORLD_HEIGHT) - (abs(cameraU)*12);
					if(drawHeight > (SCREEN_HEIGHT/2)-5){drawHeight = (SCREEN_HEIGHT/2)-5;}
					SDL_RenderDrawLine(renderer, x, drawHeight, x, SCREEN_HEIGHT-(drawHeight));
				}
			}

			// Draw Minimap
			if(false){
				int MMScale = 4;
				for (int x = 0; x < MAP_WIDTH; x++){
					for (int y = 0; y < MAP_HEIGHT; y++){
						setRenderColor(renderer, worldMap[x][y]);
						SDL_RenderDrawLine(renderer, x*MMScale, y*MMScale, x*MMScale, y*MMScale +3);
						SDL_RenderDrawLine(renderer, x*MMScale+1, y*MMScale, x*MMScale+1, y*MMScale +3);
						SDL_RenderDrawLine(renderer, x*MMScale+2, y*MMScale, x*MMScale+2, y*MMScale +3);
						SDL_RenderDrawLine(renderer, x*MMScale+3, y*MMScale, x*MMScale+3, y*MMScale +3);
						// SDL_RenderDrawLine(renderer, x*MMScale, y*MMScale, x*MMScale +3, y*MMScale);
						// SDL_RenderDrawLine(renderer, x*MMScale, y*MMScale +3, x*MMScale +3, y*MMScale +3);
						// SDL_RenderDrawLine(renderer, x*MMScale +3, y*MMScale, x*MMScale +3, y*MMScale +3);
					}
				}
				setRenderColor(renderer, 9);
				SDL_RenderDrawLine(renderer, player.pos.x*MMScale, player.pos.y*MMScale, 
											 (player.pos+player.dir+player.U).x*MMScale, 
											 (player.pos+player.dir+player.U).y*MMScale);
				SDL_RenderDrawLine(renderer, player.pos.x*MMScale, player.pos.y*MMScale, 
											 (player.pos+player.dir-player.U).x*MMScale, 
											 (player.pos+player.dir-player.U).y*MMScale);
				setRenderColor(renderer, 5);
				SDL_RenderDrawPointF(renderer, player.pos.x*MMScale, player.pos.y*MMScale);
			}
			
			// Draw flower
			drawObject(renderer, player, flower);

			SDL_RenderPresent( renderer );
			setRenderColor(renderer, 0);
			SDL_RenderClear(renderer);


			Uint64 end = SDL_GetPerformanceCounter();
			float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
			// Cap to 60 FPS
			if(16.666f - elapsedMS > 0){
				SDL_Delay((Uint32)(16.666f - elapsedMS));
			}else{printf("slowframe - %f\n", elapsedMS);}
		}

	}

	closeSDL(window, renderer, texture);

	return 0;
}