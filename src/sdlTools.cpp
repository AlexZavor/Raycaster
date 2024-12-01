#include "my_tools/sdlTools.h"
#include <stdio.h>

bool initializeSDL(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture, int width, int height){
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return 0;
	}
	//Create window
	*window = SDL_CreateWindow( "RayCaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN );
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
	*texture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
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

void handleInput(SDL_Event e, input* in){
    while( SDL_PollEvent( &e ) != 0 ) {
        switch (e.type){
            
        //User requests quit (X button)
        case SDL_QUIT:
            in->quit = true;
            break;
        //User presses a key
        case SDL_KEYDOWN:
            switch(e.key.keysym.sym){
                case SDLK_ESCAPE:
                    in->quit = true;
                break;
                case SDLK_w:
                    in->fwrd = true;
                break;
                case SDLK_s:
                    in->back = true;
                break;
                case SDLK_a:
                    in->rleft = true;
                break;
                case SDLK_d:
                    in->rright = true;
                break;
                default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch(e.key.keysym.sym){
                case SDLK_w:
                    in->fwrd = false;
                break;
                case SDLK_s:
                    in->back = false;
                break;
                case SDLK_a:
                    in->rleft = false;
                break;
                case SDLK_d:
                    in->rright = false;
                break;
                default:
                break;
            }
            break;
        
        default:
            break;
        }
    }
}

void sdlTools_SetRenderDrawColorRGBA(SDL_Renderer* renderer, rgba32 color, bool shadow){
    if(shadow){
        color = color.darken(128);
    }
    SDL_SetRenderDrawColor(renderer, color.r,color.g,color.b,color.a);
}