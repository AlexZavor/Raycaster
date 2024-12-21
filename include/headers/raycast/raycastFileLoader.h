#ifndef RAYCASTFILELOADER_H
#define RAYCASTFILELOADER_H
#pragma once

#include "raycastPlayer.h"
#include "raycastMap.h"
#include "raycastTexture.h"
#include "raycastTextureMap.h"
#include "raycastObject.h"
#include "raycastObjectMap.h"
#include <stdio.h>
#include <stdlib.h>

void raycast_loadFile(raycastMap* map, raycastPlayer* player, raycastTextureMap** textureMap, raycastObjectMap** objectMap, const char* file_dir);

#endif