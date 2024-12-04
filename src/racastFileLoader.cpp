#include "raycast/raycastFileLoader.h"

#define S_BUF_SIZE 128

void raycast_LoadPlayer(raycastPlayer* player, FILE* fp){
    float* buff = (float*)malloc(S_BUF_SIZE);

    // Output vars
    float x;
    float y;

    
    fread(buff, 4, 2, fp);
    x = buff[0];
    y = buff[1];

    free(buff);

    *player = raycastPlayer(vect2d(x,y));
}

void raycast_LoadMap(raycastMap* map, FILE* fp){

    char* buff = (char*)malloc(S_BUF_SIZE);

    // Output vars
    uint8_t x;
    uint8_t y;
    char* worldArr = NULL;
    uint8_t cp_size;
    rgba32* cp = NULL;

    
    fread(buff, 1, 2, fp);
    x = buff[0];
    y = buff[1];
    worldArr = (char*)malloc(x*y);
    fread(worldArr, 1, x*y, fp);

    fread(buff, 1, 1, fp);
    cp_size = buff[0];
    cp = (rgba32*)malloc(cp_size*sizeof(rgba32));
    fread(cp, sizeof(rgba32), cp_size, fp);

    free(buff);

    // make map for reals
    *map = raycastMap(x,y,worldArr,cp_size,cp);
    free(worldArr);
    free(cp);
}

void raycast_loadFile(raycastMap* map, raycastPlayer* player, const char* file_dir){
    FILE* fp = fopen(file_dir, "r");
    if(fp == NULL){
        printf("Can't open %s\n", file_dir);
        exit(1);
    }

    // State for reading file
    int state = 0;
    bool foundPlayer = false;
    bool foundWorld = false;
    // buffer for reading in
    char* buff = (char*)malloc(S_BUF_SIZE);
    while(fread(buff, 1, 1, fp)){
        switch (state)
        {
        case 0:
            if(buff[0] == '\0'){
                state++;
            }
            break;
        case 1:
            if(buff[0] == 'W'){state=2;}
            else if(buff[0] == 'P'){state=4;}
            else state = 0;
            break;
        case 2:
            if(buff[0] == 'L'){state++;}
            else state = 0;
            break;
        case 3:
            if(buff[0] == 'D'){
                // Found map
                raycast_LoadMap(map,fp);
                foundWorld = true;
            }
            state = 0;
            break;
        case 4:
            if(buff[0] == 'L'){state++;}
            else state = 0;
            break;
        case 5:
            if(buff[0] == 'R'){
                // Found player
                raycast_LoadPlayer(player,fp);
                foundPlayer = true;
            }
            state = 0;
            break;
        
        default:
            printf("load map sm Err \n");
            break;
        }
    }

    free(buff);

    if(!foundPlayer || !foundWorld){
        printf("File did not have needed components\n");
        exit(1);
    }
}
