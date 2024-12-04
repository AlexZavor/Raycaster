#ifndef SDLFILESELECTOR_H
#define SDLFILESELECTOR_H

#include "SDL.h"

void sdlFileSelector_selectDir(SDL_Renderer* renderer, char** file_selected, const char* directory);

#endif // SDLFILESELECTOR_H