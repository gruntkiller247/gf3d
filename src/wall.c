#include "wall.h"


Entity* spawnWall(GFC_Vector3D position, Model* model)
{
	if (!model)
	{
		slog("Cannot spawn Wall without a model!");
		return;
	}
	Entity* self = entityNew();

	if (!self)
	{
		slog("Failed to create a new Wall!");
		return;
	}

	self->model = model;
	self->position = position;

}