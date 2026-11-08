#ifndef __TILE_H__
#define __TILE_H__

#include "model.h"

typedef enum
{
	TT_ERROR = -1,
	TT_EMPTY = 0,
	TT_FLOOR,
	TT_SABER,
	TT_WALL,
	TT_FREEZE,
	TT_ENEMY_SPAWNER,


}TileTypes;


Entity* tileEntityNew(Uint8 role, GFC_Vector3D pos);

/*
	Recieves a tile
	Returns an enum if the tile can do a status effect and which
	Returns 0 on error or no status effects
	Returns int > 0 if status effect
*/
Uint8 tileCanStatus(Entity* tile);

void tileFree(Entity* self);

//void tileDraw(Entity* tile);


#endif
