#include "simple_logger.h"
#include "projectile.h"



typedef struct
{
	Uint8 moveTimer;
	Uint8 moveMax;

	GFC_Vector3D lookAt;
	Uint16 timeToLive;
	Uint16 timeToCount;

	GFC_Vector3D currentDirection;
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
	self->data = malloc(sizeof(ProjData));
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

	self->bounds = self->model->mesh->bounds;
	
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

	//slog("Projectile bounds: %f %f %f",self->bounds.w,self->bounds.h,self->bounds.d);

	//slog("MY TEAM IS: %i", self->team);
}

void projectileUpdate(Entity* self)
{
	ProjData* data;
	GFC_Vector3D moveV = { 0 };
	GFC_Vector3D offset;

	if (!self)
	{
		return;
	}

	data = (ProjData*)self->data;

	if (!data)
	{
		return;
	}

	if (data->currentDirection.x != 0)
	{
		moveV.x = data->currentDirection.x;
	}

	if (data->currentDirection.y != 0)
	{
		moveV.y = data->currentDirection.y;
	}

	if (data->currentDirection.z != 0)
	{
		moveV.z = data->currentDirection.z;
	}

	if ((moveV.x) || (moveV.y) || (moveV.z))
	{
		//slog("\nPROJ MOVE:\nX: %f\nY: %f",moveV.x,moveV.y);
		gfc_vector3d_normalize(&moveV);

		//gfc_vector3d_rotate_about_z(&moveV, data->enemyRotation.x);

		self->velocity = moveV;
	}

	

	gfc_vector3d_add(self->position, self->position, self->velocity);
	offset = gfc_vector3d(0, 0, 100);


	//gfc_vector3d_rotate_about_z(&offset, data->enemyRotation.x);
	//self->rotation.z = data->enemyRotation.x;

	//gfc_vector3d_rotate_about_x(&offset, data->enemyRotation.y);
	//self->rotation.y = data->enemyRotation.y;

	if (gfc_vector3d_magnitude_squared(self->velocity) > 0.1)
	{
		gfc_vector3d_scale(self->velocity, self->velocity, 0.8);
	}
	else
	{
		//slog("Clearing!");
		gfc_vector3d_clear(self->velocity);
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

void projectileTouch(Entity* self, Entity* toucher)
{
	if (!self)
	{
		return;
	}
	
}


/*
	Recieves itself and a to move towards direction
*/
void projectileGiveMove(Entity* self, GFC_Vector3D direction)
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

	data->currentDirection = direction;
}