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
	GFC_Box bounds;				//Don't let anything leave this
	Mix_Music* backgroundMusic;
	GFC_TextLine musicFile;

	GFC_Matrix4 terrainMatrix;
	SJson* json;	//Keep the json around in case need to refrence
}Level;


Level* levelNew();

Level*levelLoad(const char* filename);

//World.Execute(Me)
void levelFree(Level* level);

void levelDraw(Level* level);

Level* getLevel();

Uint8 levelLineTest(Level* level, GFC_Edge3D edge, GFC_Vector3D* contactpoint);


#endif