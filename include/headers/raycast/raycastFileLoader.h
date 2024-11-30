#ifndef RAYCASTFILELOADER_H
#define RAYCASTFILELOADER_H

#include "raycastPlayer.h"
#include "raycastMap.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define S_BUF_SIZE 1024

int raycast_loadFile(char* buff, const char* file_dir){
    FILE* fp = fopen(file_dir, "r");
    size_t f_size = 0;

    if (fp != NULL)
    {
        printf("Opened file - %s\n", file_dir);
        f_size = fread(buff, 1, S_BUF_SIZE, fp);
        fclose(fp);
    }
    else printf("Can't open %s\n", file_dir);

    return f_size;
}

void raycast_LoadPlayer(raycastPlayer* player, const char* file_dir){
    // TODO: load player
    // strstr()
}

void raycast_LoadMap(raycastMap* map, const char* file_dir){

    char* buff = (char*)malloc(S_BUF_SIZE);
    int size = raycast_loadFile(buff, file_dir);

    // Output vars
    uint8_t x;
    uint8_t y;
    char* worldArr = NULL;
    uint8_t cp_size;
    char* cp = NULL;

    int state = 0;
    for(int i = 0; i < size; i++){
        switch (state)
        {
        case 0:
            if(buff[i] == '\0'){
                state++;
            }
            break;
        case 1:
            if(buff[i] == 'W'){state++;}
            else state = 0;
            break;
        case 2:
            if(buff[i] == 'L'){state++;}
            else state = 0;
            break;
        case 3:
            if(buff[i] == 'D'){state++;}
            else state = 0;
            break;
        case 4:
            // Acutally load
            x = buff[i];
            i++;
            y = buff[i];
            i++;
            worldArr = (char*)malloc(x*y);
            memcpy(worldArr, buff+i, x*y);
            i+=x*y;
            cp_size = buff[i];
            i++;
            cp = (char*)malloc(cp_size*4);
            memcpy(cp, buff+i, cp_size*4);
            i+=cp_size*4;
            break;
        
        default:
            printf("load map sm Err \n");
            break;
        }
        if(worldArr != NULL){
            break;
        }
    }
    free(buff);

    // make map for reals
    *map = raycastMap(x,y,worldArr,cp_size,cp);
    free(worldArr);
    free(cp);
}

#endif