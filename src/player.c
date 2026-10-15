#include "simple_logger.h"

#include "gfc_vector.h"
#include "gfc_input.h"
#include "gfc_matrix.h"
#include "gf3d_vgraphics.h"

#include "player.h"
#include "entity.h"
#include "model.h"


typedef struct
{
	float cameraFollowDistance;
	GFC_Vector2D cameraRotation;
	GFC_Vector3D spawn;

}PlayerData;

void playerFree(Entity* self);

Entity* spawnPlayer(GFC_Vector3D position)
{
	Entity* self;
	self = entityNew();

	if (!self)
	{	
		slog("Failed to make the player");
		return NULL;
	}

	self->model = model_load("models/dino/dino.model");
	self->position = position;
	self->think = playerThink;
	self->update = playerUpdate;

	self->data = gfc_allocate_array(sizeof(PlayerData), 1);
	self->free = playerFree;

	PlayerData* data = (PlayerData*)self->data;

	if (data)
	{
		data->cameraFollowDistance = 50;
		data->cameraRotation.x = 0;
		data->cameraRotation.y = GFC_PI/6;
	}

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

int playerThink(Entity* self)
{
	GFC_Vector3D move = { 0 };
	if (!self || !self->data)
	{
		slog("Player does not exist!");
		return NULL;
	}

	GFC_Matrix4* view;
	view = gf3d_vgraphics_get_view_matrix();
	

	GFC_Vector3D offset = { 0,-50,0 };
	gfc_vector3d_add(offset, self->position,offset);
	
	gfc_matrix4_view(*view, offset, self->position, gfc_vector3d(0,0,1));

	PlayerData* data = (PlayerData*)self->data;

	//slog("Player Update!");

	if (gfc_input_command_down("panleft"))
	{
		//slog("Up!");
		data->cameraRotation.x += 0.5;

		//GFC_RADTODEG Human degree, computer Radians
	}

	if (gfc_input_command_down("panright"))
	{
		data->cameraRotation.x -= 0.5;
	}

	if (gfc_input_command_down("panup"))
	{
		data->cameraRotation.y += 0.5;
	}

	if (gfc_input_command_down("pandown"))
	{
		data->cameraRotation.y -= 0.5;
	}




	if (gfc_input_command_down("item_up"))
	{
		slog("Up!");
		move.y = 1;
	}

	if (gfc_input_command_down("item_down"))
	{
		//slog("Down!");
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


	if ((move.x) || (move.y))
	{
		gfc_vector3d_normalize(&move);

		gfc_vector3d_rotate_about_z(&move, data->cameraRotation.x);


		self->velocity = move;
	}
	gfc_vector3d_add(self->position, self->position, self->velocity);

	gfc_vector3d_scale(offset, offset, data->cameraFollowDistance);
	//gfc_vector3d_rotation();
	gfc_vector3d_rotate_about_z(&offset,data->cameraRotation.x);

	self->rotation.z = data->cameraRotation.x;

	if (gfc_vector3d_magnitude_squared(self->velocity) < 0.1)
	{
		gfc_vector3d_scale(self->velocity, self->velocity, 0.8);
	}
	else
	{
		slog("Clearing!");
		gfc_vector3d_clear(self->velocity);
	}

	return 1;
}

int playerUpdate(Entity* self)
{
	
	if (!self || !self->data)
	{
		slog("Player does not exist in update!");
		return NULL;
	}



	//gfc_input_key_pressed();
	
	return 1;
}