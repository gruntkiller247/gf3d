#include "simple_logger.h"
#include "projectile.h"



typedef struct
{
	Uint8 moveTimer;
	Uint8 moveMax;

	GFC_Vector3D lookAt;
	Uint16 timeToLive;
	Uint16 timeToCount;

}ProjData;

void projectileThink(Entity* self);
void projectileUpdate(Entity* self);
void projectileFree(Entity* self);
void projectileTouch(Entity* self);


Entity* projectileEntityNew(GFC_Vector3D position, Uint8 team, Uint16 timeToLive, Uint16 role)
{
	Entity* self;
	ProjData* data;
	self = entityNew();

	if (!self)
	{
		slog("Failed to make a proj!");
		return NULL;
	}

	data = (ProjData*)self->data;

	if (!data)
	{
		slog("Failed to make proj data!");
		return NULL;
	}

	self->position = position;
	self->team = team;
	self->role = role;
	self->isProj = 1;

	
	self->think = projectileThink;
	self->update = projectileUpdate;
	self->touch = projectileTouch;
	self->free = projectileFree;

	self->model = model_load("models/Cube/Cube.json");


	data->timeToLive = timeToLive;
	data->timeToCount = 0;

	return self;
}

void projectileThink(Entity* self)
{
	ProjData* data;
	
	if (!self)
	{
		return;
	}

	data = (ProjData*)self->data;

	if (!data)
	{
		return;
	}

	data->timeToCount++;

	if (data->timeToCount >= data->timeToLive)
	{
		self->_inUse = 0;
		return;
	}
}

void projectileUpdate(Entity* self)
{
	if (!self)
	{
		return;
	}
}

void projectileFree(Entity* self)
{
	if (!self)
	{
		return;
	}

	if (self->data)
	{
		free(self->data);
	}
}

void projectileTouch(Entity* self)
{
	if (!self)
	{
		return;
	}


}