#include "raycast/raycastObject.h"
#include "raycast/raycastRayCalc.h"

#include "stdio.h"


raycastObject::raycastObject(SDL_Renderer* renderer, vect2d position, const char* image, int scale = OBJ_DEFAULT_SCALE){
    pos = position;
    this->scale = scale;
    surface = IMG_Load(image);
    w = surface->w;
    h = surface->h;
    color = rgba32(0);
    pixels = (rgba32*)surface->pixels;
    reloadTexture(renderer);
}

raycastObject::raycastObject(uint8_t w, uint8_t h, rgba32* pixels, float x, float y, rgba32 color, int scale = OBJ_DEFAULT_SCALE){
        pos = vect2d(x,y);
        this->scale = scale;
        this->pixels = (rgba32*)malloc(w*h*sizeof(rgba32));
        surface = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
        SDL_memcpy(surface->pixels, pixels, w*h*sizeof(rgba32));
        // SDL_memcpy(this->pixels, pixels, w*h*sizeof(rgba32));
        // surface = SDL_CreateRGBSurfaceFrom(this->pixels, w, h, 32, w*4, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
        // SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));
        this->w = w;
        this->h = h;
        this->color = color;
        texture = SDL_CreateTextureFromSurface(SDL_GetRenderer(SDL_GetWindowFromID(2)), surface);
        if(texture == NULL){
            printf("Error creating texture: %s\n", SDL_GetError());
        }
    }

void raycastObject::drawObject(SDL_Renderer* renderer, raycastPlayer player, raycastMap map) {
    // get screen dimensions
    static SDL_DisplayMode DM = {0};
    if(DM.h == 0){SDL_GetCurrentDisplayMode(0, &DM);}
    static int width = DM.w;
    static int height = DM.h;

	vect2d objectDir = (player.pos-pos); 
	vect2d weirdPlayer = player.dir;
	weirdPlayer.rotate(PI_F/2);	// Makes ratio of dot product more useful for mapping across screen

	float ang = weirdPlayer.dotProduct(objectDir);
	float view = weirdPlayer.normal().dotProduct((player.dir+player.U).normal());

	if(ang > view && ang < -view && player.dir.dotProduct(objectDir)<.5){   // Check if in view
		float dist = (objectDir).getMag();
		float walldist; rgba32 tar; bool side;
		calcRay(player,&map,objectDir.normal().negative(),&walldist, &tar, &side);

		if(dist<walldist){										            // Check if not behind wall
			int drawX = -(ang * (width) / (abs(view*2))) + width/2 - (w/2);
			int drawHeight = (scale/dist) + (height/2);
			if(drawHeight > (height/2)+(h/2)){drawHeight = (height/2)+(h/2);}

			SDL_Rect texr; texr.x = drawX; texr.y = drawHeight; 
						   texr.w = w*scale/dist; texr.h = h*scale/dist; 
			if(SDL_RenderCopy(renderer, texture, NULL, &texr)){
                printf("Error drawing object: %s\n", SDL_GetError());
            }
		}
	}
}