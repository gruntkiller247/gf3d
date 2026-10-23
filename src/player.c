#include "simple_logger.h"

#include "gfc_vector.h"
#include "gfc_input.h"
#include "gfc_matrix.h"
#include "gf3d_vgraphics.h"

#include "player.h"
#include "entity.h"
#include "model.h"
#include "level.h"
#include "camera.h"

static Level* theLevel = NULL;
//Setup gets + setters!



void playerFree(Entity* self);

Entity* playerEntityNew(GFC_Vector3D position)
{
	Entity* self;
	Entity* camera;
	PlayerData* data;

	self = entityNew();

	if (!self)
	{	
		slog("Failed to make the player");
		return NULL;
	}

	//self->model = model_load("models/dino/dino.model");
	self->position = position;
	self->think = playerThink;
	self->update = playerUpdate;

	self->data = gfc_allocate_array(sizeof(PlayerData), 1);
	self->free = playerFree;


	data = (PlayerData*)self->data;

	if (!data)
	{
		slog("Failed to make Player Data!");
		return NULL;
	}


	//Camera needs to think after the player!
	data->camera = cameraEntityNew();
	data->playerRotation.x = 0;
	data->playerRotation.y = GFC_PI / 6;

	if (!data->camera)
	{
		slog("Failed to make the player's camera!");
		return NULL;
	}

	
	setPlayer(self);
	strcpy(self->name, "DINOSAUR!");
	
	return self;
}

void playerFree(Entity* self)
{
	if (!self || !self->data)
	{
		slog("Cannot free NULL player!");
		return;
	}
	PlayerData *data = (PlayerData*)self->data;

	free(data);
	self->data = NULL;
}

void playerThink(Entity* self)
{
	GFC_Vector3D move = { 0 };
	GFC_Vector3D floor;
	GFC_Vector3D offsetEdge;
	GFC_Edge3D edge;
	PlayerData* data;
	Entity* camera;
	CameraData* cData;

	if (!self || !self->data)
	{
		slog("Player does not exist!");
		return NULL;
	}

	data = (PlayerData*)self->data;
	

	if (!data)
	{
		slog("Player does not have their data!");
		return;
	}
	

	camera = getCamera();

	if (!camera)
	{
		slog("The Player could not find the camera!");
		return;
	}

	cData = (CameraData*)camera->data;

	if (!cData)
	{
		slog("Player failed to find the camera's data!");
		return;
	}


	if (gfc_input_command_down("panleft"))
	{
		data->playerRotation.x += 0.05;
	}

	if (gfc_input_command_down("panright"))
	{
		data->playerRotation.x -= 0.05;
	}

	if (gfc_input_command_down("pandown"))
	{
		data->playerRotation.y += 0.05;
	}

	if (gfc_input_command_down("panup"))
	{
		data->playerRotation.y -= 0.05;
	}
	
	//Player Movement
	if (gfc_input_command_down("item_up"))
	{
		move.y = 1;
	}

	if (gfc_input_command_down("item_down"))
	{
		move.y = -1;
	}

	if (gfc_input_command_down("item_left"))
	{
		move.x = 1;
	}

	if (gfc_input_command_down("item_right"))
	{
		move.x = -1;
	}
	
	gfc_vector3d_add(self->position, self->position, self->velocity);
	GFC_Vector3D offset = gfc_vector3d(0, 0, 100);

	//Player Movement Update - Done in Think, yeah I know!
	if ((move.x) || (move.y))
	{
		gfc_vector3d_normalize(&move);

		gfc_vector3d_rotate_about_z(&move, data->playerRotation.x);


		self->velocity = move;
	}

	gfc_vector3d_add(self->position, self->position, self->velocity);

	gfc_vector3d_scale(offset, offset, cData->cameraFollowDistance);
	gfc_vector3d_rotate_about_z(&offset, data->playerRotation.x);

	self->rotation.z = data->playerRotation.x;

	gfc_vector3d_rotate_about_x(&offset, data->playerRotation.y);
	self->rotation.y = data->playerRotation.y;

	if (gfc_vector3d_magnitude_squared(self->velocity) < 0.1)
	{
		gfc_vector3d_scale(self->velocity, self->velocity, 0.8);
	}
	else
	{
		//slog("Clearing!");
		gfc_vector3d_clear(self->velocity);
	}
}

void playerUpdate(Entity* self)
{
	
	if (!self || !self->data)
	{
		slog("Player does not exist in update!");
		return NULL;
	}



	//gfc_input_key_pressed();
	
	
}