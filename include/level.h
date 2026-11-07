#ifndef __Level_H__
#define __Level_H__

#include "model.h"
#include "gfc_audio.h"
#include "entity.h"


typedef struct Entity_S Entity;


//Can I hold many levels in memory at once?
//1 level at a time. Make another each run. No Resource manager
typedef struct
{
	//Model *terrain;
	//Model* sky;
	//GFC_TextLine skyFile;
	//GFC_TextLine terrainFile;
	//GFC_Matrix4 terrainMatrix;

	Mix_Music* backgroundMusic;
	GFC_TextLine musicFile;

	SJson* json;	//Keep the json around in case need to refrence
	GFC_Box bounds;		//Bounds of the entire level - Should be the max width of the entire 'floor'

	Uint16 maxRoomSize;	//Max size a random room can be!
	Uint16 minRoomSize;	//Min size a random room can be!	
	

	//width = 0, height = 0 = (0,0) floorPlan is the bounds effectivly with a distance of (sizeMax /2) distance
	Uint16 sizeMax;		//Max Size of the matrix		
	Uint16 width;		//Width of the level matrix!
	Uint16 height;		//Height


	//Floor plan will be a set of ints representing a floor tile/item/etc
	GFC_List* floorPlan; //2D matrix representation of the level. In truth, a 1d Matrix
		//Floorplan is a list of Tiles
			//Each tile has the model to load and a state IE lava, normal, lab

	//Model* floor;
	//Model* wall;

	//Kehoe aid
	Uint8 *tileSet;
	GFC_Vector2I tileSize; //Make functions


	
}Level;

typedef enum
{
	LB_NO_TOUCH = 0,
	LB_MIN_X,
	LB_MIN_Y,
	LB_MIN_Z,
	LB_MAX_X,
	LB_MAX_Y,
	LB_MAX_Z
}Bounds;

typedef enum
{
	TM_EMPTY = 0

}TileMap;

typedef enum
{
	//Currently marked at max size of Uint16 in level load!
	LT_WALL,		//Edge of each room/Default space
	LT_EMPTY_FLOOR,	//Tile without an item on it?

}LevelTiles;


Level* levelNew();

Level*levelLoad(const char* filename);

Level* levelLoadTest();

//World.Execute(Me)
void levelFree(Level* level);

void levelDraw(Level* level);

Level* getLevel();

Uint8 levelLineTest(Level* level, GFC_Edge3D edge, GFC_Vector3D* contactpoint);

Uint8 levelBoundsCheck(Level* level, GFC_Box aabb);

void levelPosCheck(Level* level, Entity* self);

#endif