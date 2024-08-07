#include <stdio.h>
#include "SDL.h"
#include "SDL_ttf.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


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



int main(int argc, char* argv[]) {
	// The window we'll be rendering to
	SDL_Window* window = NULL;
	// The window renderer
	SDL_Renderer* renderer = NULL;
	// The texture to render to
    SDL_Texture* texture = NULL;
	// Event handler
	SDL_Event e;

	if(initializeSDL(&window, &renderer, &texture)){
		// Main loop logic
		bool quit = false;

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
			SDL_RenderPresent( renderer );


			Uint64 end = SDL_GetPerformanceCounter();
			float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
			// Cap to 60 FPS
			if(16.666f - elapsedMS > 0){
				SDL_Delay((Uint32)(16.666f - elapsedMS));
			}
		}

	}

	closeSDL(window, renderer, texture);

	return 0;
}