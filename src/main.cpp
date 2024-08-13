#include <stdio.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "vect2d.h"
#include "rgba.h"
#include "raycastObject.h"
#include "math.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define WORLD_HEIGHT 20

#define PI_HALF 1.57079632679


#define MAP_WIDTH 24
#define MAP_HEIGHT 24

#define FLOWER_PATH "Res/Flower.png"

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


void calcRay(vect2d player, vect2d rayDir, float* dist, int* target, bool* side){
	vect2d mapTile = vect2d((int)player.x, (int)player.y); //truncate to get start tile. 

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
		nextX = (player.x - mapTile.x) * deltaX;
	} else {			// ray on right
		stepX = 1;
		nextX = (mapTile.x + 1.0 - player.x) * deltaX;
	} if (rayDir.y < 0) {	// ray down
		stepY = -1;
		nextY = (player.y - mapTile.y) * deltaY;
	} else {				// ray up
		stepY = 1;
		nextY = (mapTile.y + 1.0 - player.y) * deltaY;
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

void setRenderColor(SDL_Renderer* renderer, rgba32 color){
	SDL_SetRenderDrawColor(renderer, color.r,color.g,color.b,color.a);
}

int playerMap(vect2d player){
	return worldMap[(int)player.x][(int)player.y];
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
		vect2d player = vect2d(12,12);
		vect2d playerDir = vect2d(-1,0);
		vect2d playerU = vect2d(0,0.50);	// Sets fov, scale dependent

		raycastObject flower = raycastObject(vect2d(3,3), FLOWER_PATH);

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
			if(fwrd){player+=(playerDir*playerspeed); if(playerMap(player) != 0){player-=(playerDir*playerspeed);}}
			if(back){player-=(playerDir*playerspeed); if(playerMap(player) != 0){player+=(playerDir*playerspeed);}}
			if(rright){playerDir.rotate(lookspeed); playerU.rotate(lookspeed);}
			if(rleft){playerDir.rotate(-lookspeed); playerU.rotate(-lookspeed);}
			
			//Update screen
			for(int x = 0; x < SCREEN_WIDTH; x++){
				float cameraU = 2 * x / double(SCREEN_WIDTH) - 1;
				vect2d rayDir = playerDir - (playerU*cameraU);

				float dist;
				int target;
				bool side;

				calcRay(player, rayDir, &dist, &target, &side);

				setRenderColor(renderer, target, side);
				int drawHeight = (dist*WORLD_HEIGHT) - (abs(cameraU)*12);
				if(drawHeight > (SCREEN_HEIGHT/2)-5){drawHeight = (SCREEN_HEIGHT/2)-5;}
				SDL_RenderDrawLine(renderer, x, drawHeight, x, SCREEN_HEIGHT-(drawHeight));
			}

			// Draw Minimap
			if(true){
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
				SDL_RenderDrawLine(renderer, player.x*MMScale, player.y*MMScale, (player+playerDir+playerU).x*MMScale, (player+playerDir+playerU).y*MMScale);
				SDL_RenderDrawLine(renderer, player.x*MMScale, player.y*MMScale, (player+playerDir-playerU).x*MMScale, (player+playerDir-playerU).y*MMScale);
				setRenderColor(renderer, 5);
				SDL_RenderDrawPointF(renderer, player.x*MMScale, player.y*MMScale);
			}
			
			// Draw flower
			if(true){
				vect2d flowerDir = (player-flower.pos); 
				vect2d weirdPlayer = playerDir;
				weirdPlayer.rotate(PI_HALF);	// Makes ratio of dot product more useful for mapping across screen

				float ang = weirdPlayer.dotProduct(flowerDir);
				float view = weirdPlayer.normal().dotProduct((playerDir+playerU).normal());

				// printf("ang %f, view %f\n", ang, view);

				if(ang > view && ang < -view && playerDir.dotProduct(flowerDir)<.5){
					float dist = (flowerDir).getMag();
					int w, h; // texture width & height

					int drawX = -(ang * (SCREEN_WIDTH) / (abs(view*2))) + SCREEN_WIDTH/2;

					SDL_Texture* flower_texture = SDL_CreateTextureFromSurface(renderer, IMG_Load(FLOWER_PATH));
					SDL_QueryTexture(flower_texture, NULL, NULL, &w, &h);
					int drawHeight = (dist*WORLD_HEIGHT) +h*20/dist;
					if(drawHeight > (SCREEN_HEIGHT/2)+(h/2)){drawHeight = (SCREEN_HEIGHT/2)+(h/2);}
					SDL_Rect texr; texr.x = drawX; texr.y = SCREEN_HEIGHT - drawHeight; texr.w = w*20/dist; texr.h = h*20/dist; 
					SDL_RenderCopy(renderer, flower_texture, NULL, &texr);
				}
			}

			SDL_RenderPresent( renderer );
			setRenderColor(renderer, 0);
			SDL_RenderClear(renderer);


			Uint64 end = SDL_GetPerformanceCounter();
			float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
			// Cap to 60 FPS
			if(16.666f - elapsedMS > 0){
				SDL_Delay((Uint32)(16.666f - elapsedMS));
			}else{printf("%f\n", elapsedMS);}
		}

	}

	closeSDL(window, renderer, texture);

	return 0;
}