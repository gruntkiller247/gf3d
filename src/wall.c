#include "wall.h"

void wallTouch(Entity* self, Entity* toucher);

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
	self->touch = wallTouch;

	return self;

}

void wallTouch(Entity* self, Entity* toucher)
{
	if (!self || !toucher)
	{
		return;
	}

	//Do Wall touching!

	if(toucher->team != TEAM_WALL)
	{
		slog("Something is touching a wall!");
		hitWall(toucher);
	}
}