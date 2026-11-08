#include "simple_logger.h"
#include "entity.h"
#include "tile.h"
#include "projectile.h"


typedef struct
{
	Uint8 canFreeze;
	Uint16 shotTimer;
	Uint16 shotMax;
}TileData;


void tileDraw(Entity* tile);
void tileUpdate(Entity* self);


Entity* tileEntityNew(Uint8 role, GFC_Vector3D pos)
{
	Entity* tile;
	TileData* data;
	int rdNum;

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
	//Initalize all data stuff here! Or else the freeze bug comes back!
	data->canFreeze = 0;
	data->shotMax = -1;
	data->shotTimer = 0;
	

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

			rdNum = (rand() % (2000 - 1000 + 1)) + 1000;

			data->shotMax = rdNum;
			//slog("RDNUM: %i", rdNum);
			tile->team = TEAM_TRAP;
			break;

		default:
			slog("No Tile role type!");

			break;
	}

	tile->role = role;
	tile->free = tileFree;
	tile->update = tileUpdate;

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

void tileUpdate(Entity* self)
{
	TileData* data;
	Entity* proj;

	if (!self)
	{
		return;
	}

	data = (TileData*)self->data;

	if (!data)
	{
		return;
	}

	if (tileCanStatus(self) != 0)
	{
		data->shotTimer++;
		//Do tile 'shot' above
		if (data->shotTimer >= data->shotMax)
		{
			//slog("SHOOTING ICE SPIKE!\nShotMax: %i",data->shotMax);
			data->shotTimer = 0;
			proj = projectileEntityNew(self->position, self->team, TTL_PROJ_MELEE, ROLE_FREEZE_PROJ);
		}
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

	if (data->canFreeze == 1)
	{
		return SE_FREEZE;
	}
	else
	{
		return SE_NONE;
	}
}
