#include "simple_logger.h"
#include "entity.h"
#include "tile.h"



void tileDraw(Entity* tile);


Entity* tileEntityNew(int role, GFC_Vector3D pos)
{
	Entity* tile;

	tile = entityNew();

	if (!tile)
	{
		slog("Failed to make tile!");
		return NULL;
	}

	tile->role = ROLE_FLOOR;
	//tile->draw = tileDraw;

	gfc_vector3d_copy(tile->position,pos);
	
	switch (role)
	{
	case TT_EMPTY:
		tile->model = model_load("models/floor/floor.json");
		break;

	case TT_SABER:
		tile->model = model_load("models/Cube/Cube.json");
		break;

	case TT_ERROR:
		slog("Error Case!");
		break;

	default:
		slog("No Tile role type!");

		break;
	}

	tile->bounds = tile->model->mesh->bounds;

	return tile;
}

void tileDraw(Entity* tile)
{
	if (!tile)
	{
		return;
	}
}
