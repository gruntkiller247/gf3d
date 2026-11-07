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
void playerTouch(Entity* self, Entity* toucher);
void playerDraw(Entity* self);

Entity* playerEntityNew(GFC_Vector3D position)
{
	Entity* self;
	Entity* camera;
	PlayerData* data;

	if (thePlayer)
		return thePlayer;

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

	self->data = malloc(sizeof(PlayerData));
	self->free = playerFree;
	self->touch = playerTouch;
	self->draw = playerDraw;
	self->team = TEAM_PLAYER;
	self->role = ROLE_PLAYER;
	self->velocityLastFrame = gfc_vector3d(0,0,0);


	//GFC Box has a bounds. x,y,z w,h,d
	//x,y,z are the minimum positions of the box
	//w,h,d are the routes to the max pos
	//maxX,maxY,maxZ = x+w, y+h, z+d

	self->bounds = self->model->mesh->bounds;

	data = (PlayerData*)self->data;

	if (!data)
	{
		slog("Failed to make Player Data!");
		return NULL;
	}


	//Camera needs to think after the player!
	data->camera = cameraEntityNew();
	

	if (!data->camera)
	{
		slog("Failed to make the player's camera!");
		return NULL;
	}
	
	/*
	data->camera->position.x = self->position.x;
	data->camera->position.y = self->position.y;
	data->camera->position.z = self->position.z;
	*/

	data->playerRotation.x = 0;
	data->playerRotation.y = GFC_PI / 6;
	data->panSpeed = 0.05f;
	
	setPlayer(self);
	strcpy(self->name, "Player!");
	
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
		if (data->playerRotation.x + data->panSpeed >= 2* GFC_PI)
		{
			//Player is doing the hookie pookie and turning around!
			data->playerRotation.x = 0;
		}
		else
		{
			//Player is mid hookie pookie!
			data->playerRotation.x += data->panSpeed;
		}
		
	}

	if (gfc_input_command_down("panright"))
	{
		if (data->playerRotation.x - data->panSpeed <= 0)
		{
			//Player is doing the hookie pookie and turning around!
			data->playerRotation.x = 2* GFC_PI;
		}
		else
		{
			//Player is mid hookie pookie!
			data->playerRotation.x -= data->panSpeed;
		}
	}

	if (gfc_input_command_down("pandown"))
	{
		if (data->playerRotation.y + data->panSpeed >= GFC_PI / 2)
		{
			//Do not let happen
		}
		else
		{
			data->playerRotation.y += data->panSpeed;
		}
		
	}

	if (gfc_input_command_down("panup"))
	{
		if (data->playerRotation.y - data->panSpeed <= -1*(GFC_PI / 2))
		{
			//Do not let happen
		}
		else
		{
			data->playerRotation.y -= data->panSpeed;
		}
		
	}

	//slog("\nPlayerRotation x: %f\nPlayerRotation y: %f", data->playerRotation.x, data->playerRotation.y);
	
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
		move.x = -1;
	}

	if (gfc_input_command_down("item_right"))
	{
		move.x = 1;
	}
	

	GFC_Vector3D offset = gfc_vector3d(0, 0, 100);

	//Player Movement Update - Done in Think, yeah I know!
	if ((move.x) || (move.y))
	{
		gfc_vector3d_normalize(&move);

		gfc_vector3d_rotate_about_z(&move, data->playerRotation.x);

		
		//self->velocityLastFrame = self->velocityLastFrame;
		self->velocity = move;
		
	}

	gfc_vector3d_add(self->position, self->position, self->velocity);

	gfc_vector3d_scale(offset, offset, cData->cameraFollowDistance);
	gfc_vector3d_rotate_about_z(&offset, data->playerRotation.x);

	self->rotation.z = data->playerRotation.x;

	gfc_vector3d_rotate_about_x(&offset, data->playerRotation.y);
	self->rotation.y = data->playerRotation.y;

	if (gfc_vector3d_magnitude_squared(self->velocity) > 0.1)
	{
		gfc_vector3d_scale(self->velocity, self->velocity, 0.8);
	}
	else
	{
		//slog("Clearing!");
		gfc_vector3d_clear(self->velocity);
	}
	//slog("Player's current velocity: %f,%f,%f\nPlayer's last frame velocity: %f,%f,%f", self->velocity.x, self->velocity.y, self->velocity.z, self->velocityLastFrame.x, self->velocityLastFrame.y, self->velocityLastFrame.z);

	//slog("\nPlayer's spacial position x: %f\nPlayer's spacial position y: %f",self->position.x,self->position.y);
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

void playerDraw(Entity* self)
{
	if (!self)
		return;

	//TO DO ANYTHING RELEVANT HERE!
	//FOR NOW THE PLAYER HAS NO MODEL!
}

void playerTouch(Entity* self, Entity* toucher)
{
	//Player's box
	//Min num
	float pX, pY, pZ;
	float pW, pH, pD;
	//Max num are the aboves combined

	//Toucher's box
	//Min nums
	float tX, tY, tZ;
	float tW, tH, tD;
	//Max num are the aboves combined

	if (!self || !toucher)
	{
		slog("Player is trying to touch/be touched by an error!");
		return;
	}

	pX = self->bounds.x + self->position.x;
	pY = self->bounds.y + self->position.y;
	pZ = self->bounds.z + self->position.z;
	pW = self->bounds.w;
	pH = self->bounds.h;
	pD = self->bounds.d;

	tX = toucher->bounds.x + toucher->position.x;
	tY = toucher->bounds.y + toucher->position.y;
	tZ = toucher->bounds.z + toucher->position.z;
	tW = toucher->bounds.w;
	tH = toucher->bounds.h;
	tD = toucher->bounds.d;

	//Check if we are touching
	if (pX <= tX + tW && pX + pW >= tX && pY <= tY + tH && pY + pH >= tY && pZ <= tZ + tD && pZ + pD >= tZ)
	{
		//Calculate overlap on each axis
		float overlapX = (pX + pW) - tX;
		float overlapY = (pY + pH) - tY;
		float overlapZ = (pZ + pD) - tZ;

		//Also calculate overlap from the opposite direction
		float overlapX2 = (tX + tW) - pX;
		float overlapY2 = (tY + tH) - pY;
		float overlapZ2 = (tZ + tD) - pZ;

		if (overlapX > overlapX2)
		{
			overlapX = overlapX2;
		}

		if (overlapY > overlapY2)
		{
			overlapY = overlapY2;
		}

		if (overlapZ > overlapZ2)
		{
			overlapZ = overlapZ2;
		}

		if (overlapX <= overlapY && overlapX <= overlapZ)
		{
			//Resolve X collision
			if (pX < tX)
				self->position.x -= overlapX;
			else
				self->position.x += overlapX;
		}
		else if (overlapY <= overlapX && overlapY <= overlapZ)
		{
			//Resolve Y collision
			if (pY < tY)
				self->position.y -= overlapY;
			else
				self->position.y += overlapY;
		}
		else
		{
			/*
			//Resolve Z collision
			if (pZ < tZ)
				self->position.z -= overlapZ;
			else
				self->position.z += overlapZ;
			*/
		}
	}
	else
	{
		//Not touching on the X
	}

}