#include <stdio.h>
#include "my_tools/sdlTools.h"
#include "raycastEngine.h"
#include <vector>

// #define AVGFPS
#ifdef AVGFPS
	static double maxFtime = 0;
	static long frames = 0;
#endif

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define MAP_PATH "Res/Map.png"
#define FLOWER_PATH "Res/Flower.png"
#define WALL_TEXUTRE_PATH "Res/WallTexture.png"

int main(int argc, char* argv[]) {
	// The window we'll be rendering to
	SDL_Window* window = NULL;
	// The window renderer
	SDL_Renderer* renderer = NULL;
	// The texture to render to
    SDL_Texture* texture = NULL;
	// Event handler
	SDL_Event e;
	// Input map
	input in;

	if(initializeSDL(&window, &renderer, &texture, SCREEN_WIDTH, SCREEN_HEIGHT)){
		// required for the game
		framebuffer fbuff = framebuffer(texture, SCREEN_WIDTH, SCREEN_HEIGHT);
		raycastMap map = raycastMap();
		// raycast_LoadMap(&map, "Res/TestWorld.lvl");
		raycastPlayer player = raycastPlayer(vect2d(2,2));

		// load lvl file
		raycast_loadFile(&map, &player, "Res/TestWorld.lvl");

		// Objects
		std::vector<raycastObject> objectVector;
		// objectVector.push_back(raycastObject(renderer, vect2d(3,3), FLOWER_PATH));

		// Textures
		std::vector<raycastTexture> textureMap;
		// textureMap.push_back(raycastTexture(WALL_TEXUTRE_PATH, rgba32(0,19,230)));

		while(!in.quit){
			// Frame Timer
			Uint64 start = SDL_GetPerformanceCounter();

			// Handle events on queue (Input)
			handleInput(e, &in);

			//Update player
			updatePlayer(&player, in, map);
			
			//Update screen (Raycasting)
			raycastScreen( player, &map, &fbuff, &textureMap );
			fbuff.pushFrame( renderer );

			// Draw Minimap (.1ms)
			if(DRAW_MAP)
				map.drawMap( renderer, player );
			
			// Draw Objects (very fast)
			for(unsigned int i = 0; i < objectVector.size(); i++){
				objectVector[i].drawObject(renderer, player, map);
			}

			SDL_RenderPresent( renderer );

			// Timing
			Uint64 end = SDL_GetPerformanceCounter();
			float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
			#ifdef AVGFPS
				frames++;
				maxFtime += elapsedMS;
			#endif
			// Cap to 60 FPS
			if(16.666f*2 - elapsedMS > 0){
				SDL_Delay((Uint32)(16.666f*2 - elapsedMS));
			}else{printf("slowframe - %f\n", elapsedMS);}
		}
	}

	#ifdef AVGFPS
		printf("Average Frame Time - %fms\n", maxFtime/frames);
	#endif
	closeSDL(window, renderer, texture);

	return 0;
}

/*Timer for debugging
			Uint64 quickcountS = SDL_GetPerformanceCounter();
			Uint64 quickcountE = SDL_GetPerformanceCounter();
			printf("quickcount - %f\n",(quickcountE - quickcountS) / (float)SDL_GetPerformanceFrequency() * 1000.0f);
*/
