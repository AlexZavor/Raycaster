#include <stdio.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "vect2d.h"
#include "math.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define WORLD_HEIGHT 20


#define MAP_WIDTH 24
#define MAP_HEIGHT 24

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
	SDL_Quit();
	return true;
}


void calcRay(vect2d player, vect2d rayDir, float* dist, int* target){
	vect2d mapTile = vect2d((int)player.x, (int)player.y); //truncate to get start tile. 

	float slopeY = (rayDir.y*rayDir.y)/(rayDir.x*rayDir.x);
	float deltaX = sqrt(1+(1/slopeY));
	float deltaY = sqrt(1+slopeY);

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
			nextX += deltaX;
			mapTile.x += stepX;
		} else {			// hits new Y tile
			*dist = nextY;
			nextY += deltaY;
			mapTile.y += stepY;
		}
	}

	*target = worldMap[(int)mapTile.x][(int)mapTile.y];
	return;
}

void setRenderColor(SDL_Renderer* renderer, int color){
	switch (color)
	{
	case 0:
		SDL_SetRenderDrawColor(renderer, 0,0,0,255);
		break;
	case 1:
		SDL_SetRenderDrawColor(renderer, 0,0,255,255);
		break;
	case 2:
		SDL_SetRenderDrawColor(renderer, 255,0,0,255);
		break;
	case 3:
		SDL_SetRenderDrawColor(renderer, 0,255,0,255);
		break;
	
	default:
		SDL_SetRenderDrawColor(renderer, 255,45,229,255);
		break;
	}
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
		vect2d playerU = vect2d(0,0.66);	// Sets fov, scale dependent

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
						default:
						break;
					}
				}
			}
			
			//Update screen
			for(int x = 0; x < SCREEN_WIDTH; x++){
				float cameraU = 2 * x / double(SCREEN_WIDTH) - 1;
				vect2d rayDir = playerDir + (playerU*cameraU);

				float dist;
				int target;

				calcRay(player, rayDir, &dist, &target);

				setRenderColor(renderer, target);
				SDL_RenderDrawLine(renderer, x, dist*WORLD_HEIGHT, x, SCREEN_HEIGHT-(dist*WORLD_HEIGHT));
			}

			// Draw Minimap
			int minimapScale = 4;
			if(true){
				for (int x = 0; x < MAP_WIDTH; x++){
					for (int y = 0; y < MAP_HEIGHT; y++){
						setRenderColor(renderer, worldMap[x][y]);
						SDL_RenderDrawPoint(renderer,x*minimapScale, y*minimapScale);
						SDL_RenderDrawPoint(renderer,x*minimapScale +1, y*minimapScale);
						SDL_RenderDrawPoint(renderer,x*minimapScale +2, y*minimapScale);
						SDL_RenderDrawPoint(renderer,x*minimapScale +3, y*minimapScale);
						SDL_RenderDrawPoint(renderer,x*minimapScale, y*minimapScale +1);
						SDL_RenderDrawPoint(renderer,x*minimapScale, y*minimapScale +2);
						SDL_RenderDrawPoint(renderer,x*minimapScale, y*minimapScale +3);
						SDL_RenderDrawPoint(renderer,x*minimapScale +3, y*minimapScale +1);
						SDL_RenderDrawPoint(renderer,x*minimapScale +3, y*minimapScale +2);
						SDL_RenderDrawPoint(renderer,x*minimapScale +3, y*minimapScale +3);
						SDL_RenderDrawPoint(renderer,x*minimapScale +1, y*minimapScale +3);
						SDL_RenderDrawPoint(renderer,x*minimapScale +2, y*minimapScale +3);
						SDL_RenderDrawPoint(renderer,x*minimapScale +3, y*minimapScale +3);
					}
				}
			}
			
			SDL_RenderPresent( renderer );
			// SDL_SetRenderDrawColor(renderer, 100,100,100,255);
			// SDL_RenderDrawRect(renderer, NULL);


			Uint64 end = SDL_GetPerformanceCounter();
			float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
			printf("%f\n", elapsedMS);
			// Cap to 60 FPS
			if(16.666f - elapsedMS > 0){
				SDL_Delay((Uint32)(16.666f - elapsedMS));
			}
		}

	}

	closeSDL(window, renderer, texture);

	return 0;
}