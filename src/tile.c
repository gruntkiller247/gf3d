#include "simple_logger.h"
#include "entity.h"
#include "tile.h"



typedef struct
{
	Uint8 canFreeze;
	Uint8 shotTimer;
	Uint8 shotMax;
}TileData;


void tileDraw(Entity* tile);


Entity* tileEntityNew(Uint8 role, GFC_Vector3D pos)
{
	Entity* tile;
	TileData* data;

	tile = entityNew();

	if (!tile)
	{
		slog("Failed to make tile!");
		return NULL;
	}

	tile->data = malloc(sizeof(TileData));
	data = (TileData*)tile->data;

	if (!data)
	{
		slog("Failed to make tile data!");
		return NULL;
	}
	
	//tile->draw = tileDraw;

	gfc_vector3d_copy(tile->position,pos);
	
	switch (role)
	{
		case TT_EMPTY:
			//tile->model = model_load("models/floor/floor.json");
			break;

		case TT_SABER:
			tile->model = model_load("models/Cube/Cube.json");
			break;

		case TT_ERROR:
			slog("Error Case!");
			break;

		case TT_FLOOR:
			//tile->role = ROLE_FLOOR;
			tile->model = model_load("models/floor/floor.json");
			break;

		case TT_FREEZE:
			//tile->role = ROLE_FLOOR;
			tile->model = model_load("models/floor/floor.json");
			tile->color = GFC_COLOR_LIGHTBLUE;
			data->canFreeze = 1;
			break;

		default:
			slog("No Tile role type!");

			break;
	}

	tile->role = role;
	tile->free = tileFree;

	if (tile->model)
	{
		tile->bounds = tile->model->mesh->bounds;
	}
	

	return tile;
}

void tileDraw(Entity* tile)
{
	if (!tile)
	{
		return;
	}
}

void tileFree(Entity* tile)
{
	if (!tile)
	{
		return;
	}

	if (tile->data)
	{
		free(tile->data);
	}
}

Uint8 tileCanStatus(Entity* tile)
{
	TileData* data;

	if (!tile)
	{
		return SE_NONE;
	}

	data = (TileData*)tile->data;

	if (!data)
	{
		return SE_NONE;
	}

	if (data->canFreeze)
	{
		return SE_FREEZE;
	}
	else
	{
		return SE_NONE;
	}
}
