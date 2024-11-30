#include "my_tools/framebuffer.h"
#include <stdio.h>

framebuffer::framebuffer(SDL_Texture* texture, int screenWidth, int screenHeight){
    this->texture = texture;
    this->screenHeight = screenHeight;
    this->screenWidth = screenWidth;
    frameBuffer = (rgba32*)malloc(screenWidth*screenHeight*sizeof(rgba32)); //yay memory allocation
}

bool framebuffer::pushFrame(SDL_Renderer *renderer){
    // Lock the memory in order to write our Back Buffer image to it
    if (!SDL_LockTexture(texture, NULL, (void**)&pixelBuffer, &pitch))
    {
        // The pitch of the Back Buffer texture in VRAM must be divided by four bytes
        // as it will always be a multiple of four
        pitch /= sizeof(uint32_t);

        // Draw frame to texture
        SDL_memcpy(pixelBuffer, frameBuffer, screenWidth*screenHeight*sizeof(rgba32));

        // Unlock the texture in VRAM and send to renderer!
        SDL_UnlockTexture(texture);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        // Clears internal buffer
        SDL_memset(frameBuffer, 0, screenWidth*screenHeight*sizeof(rgba32));
        return true;
    }
    //Failed to lock texture
    printf("Failed to lock texture\n");
    SDL_memset(frameBuffer, 0, screenWidth*screenHeight*sizeof(rgba32));
    return false;
}

void framebuffer::drawVline(int x, int y1, int y2, rgba32 color){
    // swap so y2 is always larger
    if(y2<y1){int y = y1; y1 = y2; y2 = y;}
    // Clamps to screen
    if(y1<0){y1=0;}
    if(y2>screenHeight){y2=screenHeight;}
    // draw line
    for(int y = y1; y<y2; y++){
        frameBuffer[x+(y*screenWidth)] = color;
    }
}

framebuffer::~framebuffer()
{
    free(frameBuffer);
}
