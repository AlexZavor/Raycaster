#ifndef RAYCASTFILELOADER_H
#define RAYCASTFILELOADER_H
#pragma once

#include "raycastPlayer.h"
#include "raycastMap.h"
#include <stdio.h>
#include <stdlib.h>

void raycast_LoadPlayer(raycastPlayer* player, const char* file_dir);

void raycast_LoadMap(raycastMap* map, FILE* fp);

void raycast_loadFile(raycastMap* map, raycastPlayer* player, const char* file_dir);

#endif