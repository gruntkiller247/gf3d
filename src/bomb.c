#include "simple_logger.h"
#include "projectile.h"
#include "bomb.h"

typedef struct
{
	Uint16 timeToLive;
	Uint16 timeToCount;

	Uint16 numExplosions;
}BombData;

void bombThink(Entity* self);
void bombUpdate(Entity* self);
void bombFree(Entity* self);
void bombTouch(Entity* self, Entity* toucher);
void explode(Entity* self);

Entity* bombEntityNew(GFC_Vector3D position, Uint8 team, Uint16 timeToLive, Uint16 role)
{
	Entity* self;
	BombData* data;

	self = entityNew();

	if (!self)
	{
		slog("Failed to make a bomb!");
		return NULL;
	}

	self->data = malloc(sizeof(BombData));

	data = (BombData*)self->data;

	if (!data)
	{
		slog("Failed to make bomb data!");
		return NULL;
	}

	switch (role)
	{
		case ROLE_LAVA_BOMB:
			data->numExplosions = 3;
			break;

		default:
			data->numExplosions = 1;
	}

	switch (timeToLive)
	{
		case TTL_BOMB_MELEE:
			self->ignoreGravity = 1;

			break;

		default:

			break;
	}

	self->position = position;
	self->team = team;
	self->role = role;

	self->model = model_load("models/Cube/Cube.json");

	self->bounds = self->model->mesh->bounds;

	self->think = bombThink;
	self->touch = bombTouch;
	self->free = bombFree;
	self->update = bombUpdate;

	data->timeToLive = timeToLive;
	data->timeToCount = 0;

	
	return self;
}

void bombThink(Entity* self)
{
	BombData* data;

	if (!self)
	{
		return;
	}
	

	data = (BombData*)self->data;

	if (!data)
	{
		slog("Fail data 1");
		return;
	}

	//slog("Time: %i",data->timeToCount);
	
	data->timeToCount++;
	
}

void bombUpdate(Entity* self)
{
	BombData* data;
	if (!self)
	{
		return;
	}

	data = (BombData*)self->data;

	if (!data)
	{
		slog("Fail data");
		return;
	}
	//slog("Time: %i", data->timeToCount);

	if(data->timeToCount >= data->timeToLive)
	{
		explode(self);
		return;
	}

}

void bombFree(Entity* self)
{
	if (!self)
	{
		return;
	}

	if (self->data)
	{
		free(self->data);
		self->data = NULL;
	}
}

void bombTouch(Entity* self, Entity* toucher)
{
	if (!self || !toucher)
	{
		return;
	}
}

void explode(Entity* self)
{
	BombData* data;
	Entity* proj;
	GFC_Vector3D temp = { 0 };
	int c = 0;

	if (!self)
	{
		return;
	}
	//slog("I AM THE BOMB AND I AM BLOWING UP!");

	data = (BombData*)self->data;

	if (!data)
	{
		slog("Fail data");
		return;
	}
	if (data->numExplosions > 0)
	{
		data->numExplosions--;
		data->timeToCount = 0;
	}
	else
	{
		self->_inUse = 0;
	}
	

	for (c = 0; c < 8; c++)
	{
		proj = projectileEntityNew(self->position,self->team,TTL_PROJ_DEFAULT,ROLE_DEFAULT_PROJ);

		switch (c)
		{
			case D_NORTH:
				temp.x = 1;
				break;

			case D_NORTHEAST:
				temp.x = 1;
				temp.y = 1;
				break;

			case D_EAST:
				temp.y = 1;
				break;

			case D_SOUTHEAST:
				temp.y = 1;
				temp.x = -1;
				break;

			case D_SOUTH:
				temp.x = -1;
				break;

			case D_SOUTHWEST:
				temp.x = -1;
				temp.y = -1;
				break;

			case D_WEST:
				temp.x = -1;
				break;

			case D_NORTHWEST:
				temp.x = 1;
				temp.y = -1;
				break;

			default:
				slog("Some sorta error here!");
				break;
		}

		projectileGiveMove(proj,temp);
	}
}