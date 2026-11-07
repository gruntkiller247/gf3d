#ifndef __Tile_H__
#define __Tile_H__

#include "model.h"

typedef enum
{
	TT_ERROR = -1,
	TT_EMPTY = 0,
	TT_SABER = 1
}TileTypes;


Entity* tileEntityNew(int role, GFC_Vector3D pos);

//void tileDraw(Entity* tile);


#endif
