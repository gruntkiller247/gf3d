#ifndef __Level_H__
#define __Level_H__

#include "model.h"
#include "gfc_audio.h"


//Can I hold many levels in memory at once?
//1 level at a time. Make another each run. No Resource manager
typedef struct
{
	Model *terrain;
	Model* sky;
	GFC_TextLine skyFile;
	GFC_TextLine terrainFile;
	GFC_Box bounds;				
	Mix_Music* backgroundMusic;
	GFC_TextLine musicFile;

	GFC_Matrix4 terrainMatrix;
	SJson* json;	//Keep the json around in case need to refrence

	Uint16 maxRoomSize;	//Max size a random room can be!
	Uint16 minRoomSize;	//Min size a random room can be!
	Uint16 width;	//Size of the level matrix!
	Uint16 height;
	GFC_Matrix2 floorPlan; //2D matrix representation of the level
	
}Level;

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


#endif