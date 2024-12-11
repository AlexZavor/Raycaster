#include "my_tools/sdlFileSelector.h"
#include "my_tools/sdlTools.h"
#include "my_tools/tinydir.h"
#include "SDL_ttf.h"
#include "stdio.h"

#define FILE_VECT_SIZE 20
#define CURSOR_ARROW_H 20
#define FONT_POINT 24

void sdlFileSelector_selectDir(SDL_Renderer* renderer, char** file_selected, const char* directory){

	// Input map
	SDL_Event e;
	input in;
    bool inputOS = false; 

    int selected = 0;
    *file_selected[0] = 0; // Put in 0 if return early

    // Data structure for File names
    char** files = (char**)malloc(FILE_VECT_SIZE*sizeof(char**));
    memset(files, 0, FILE_VECT_SIZE*sizeof(char**));
    int file_index = 0;

    // Font stuff
    TTF_Font * font = TTF_OpenFont("Res/KarmaFuture.ttf", FONT_POINT);
    SDL_Color font_color = { 255, 255, 255 };
    SDL_Surface * surface;
    SDL_Texture * texture;

    // Tinydir for searching files
    tinydir_dir dir;
    tinydir_open_sorted(&dir, directory);
    // look for all files in directory
    for (unsigned int i = 0; i < dir.n_files; i++)
    {
        tinydir_file file;
        tinydir_readfile_n(&dir, &file, i);

        if (!file.is_dir)
        {
            // printf("%s", file.name);
            // printf("\n");
            files[file_index] = (char*)malloc(_TINYDIR_FILENAME_MAX);
            strcpy(files[file_index],file.name);
            file_index++;
            if(file_index == FILE_VECT_SIZE){
                break;
            }
        }
    }
    tinydir_close(&dir);

    while(!in.sel){
        // selection input
        sdlTools_handleInput(e,&in);
        if(in.fwrd && (!inputOS)){
            inputOS = true;
            selected--;
            if(selected < 0){selected = file_index-1;}
        }else if(in.back && (!inputOS)){
            inputOS = true;
            selected++;
            selected %= file_index;
        }else if((!in.back) && (!in.fwrd)){
            inputOS = false;
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);


        for(int text = 0; text < file_index; text++){
            surface = TTF_RenderText_Solid(font,
            files[text], font_color);
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            int texW = 0;
            int texH = 0;
            SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
            SDL_Rect dstrect = { 50, (text*texH)+10, texW, texH };
            SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        }
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderDrawLine(renderer, 10, (selected*32)+36, 40, (selected*32)+26);
        SDL_RenderDrawLine(renderer, 10, (selected*32)+16, 40, (selected*32)+26);
        SDL_RenderPresent(renderer);
    }

    if(files[selected] != 0){
        strcpy(*file_selected, directory);
        strcpy((*file_selected)+strlen(directory), files[selected]);
        for(int i = 0; i < FILE_VECT_SIZE; i++){
            if(files[i] == 0){
                break;
            }
            free(files[i]);
        }
        free(files);
    }
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
    return;
}