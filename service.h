#ifndef _SERVICE_H_
#define _SERVICE_H_

#include "hge.h"
#include "hgesprite.h"
#include "hgeanim.h"
#include "hgevector.h"
#include "hgeFont.h"
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <algorithm>
#include <string>
#include <fstream>
#include <memory>

// forward class declarations
class Objects;
class Scene;

// extern declarations
extern HGE* hge ;
extern Objects* objects;
extern Scene* scene;
extern float dt;
extern int toNextMap;
extern bool toExit;

// constants
const float MONSTER_SPEED = 50.0f;
const int GAME_WIDTH = 640;
const int GAME_HEIGHT = 640;
const int MRC = 20;
const float TILE_STEP = 32.0f;

// enumerations 
enum game_errors { LOAD_SCENE_SOURCES, LOAD_MONSTER_SOURCES, NULL_POINTER, OPEN_FILE, UNKNOWN_ERROR };
enum directions { NORTH_WEST, NORTH_EAST, SOUTH_WEST, SOUTH_EAST };

#endif