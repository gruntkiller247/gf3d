#include "wall.h"

void wallTouch(Entity* self, Entity* toucher);

Entity* spawnWall(GFC_Vector3D position, Model* model)
{
	if (!model)
	{
		slog("Cannot spawn Wall without a model!");
		return NULL;
	}
	Entity* self = entityNew();

	if (!self)
	{
		slog("Failed to create a new Wall!");
		return NULL;
	}

	self->model = model;
	self->position = position;
	self->bounds = self->model->mesh->bounds;
	self->team = TEAM_IGNORE;
	self -> role = ROLE_WALL;

	self->bounds = self->model->mesh->bounds;

	return self;

}
