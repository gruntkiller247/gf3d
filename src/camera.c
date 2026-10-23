#include "simple_logger.h"
#include "camera.h"
#include "gf3d_vgraphics.h"



void cameraDraw(Entity* self);


Entity* cameraEntityNew()
{
	Entity* self = entityNew();
	CameraData* data;


	if (!self)
	{
		slog("Failed to create the Camera Entity!");
		return NULL;
	}

	self->position = gfc_vector3d(0, 0, 0);
	self->rotation = gfc_vector3d(0, 0, 0);
	self->think = cameraThink;
	self->draw = cameraDraw;
	self->data = gfc_allocate_array(sizeof(CameraData), 1);

	data = (CameraData*)self->data;

	if (!data)
	{
		slog("Failed to make Camera's Data!");
		return NULL;
	}

	data->cameraFollowDistance = 50;
	data->cameraRotation.x = 0;
	data->cameraRotation.y = GFC_PI / 6;
	data->cameraOffset = gfc_vector3d(0, -50, 0);
	data->originalCameraOffset = data->cameraOffset;
	

	self->data = data;

	return self;
}

void cameraThink(Entity* self)
{
	GFC_Matrix4* view;
	Entity* player;
	int x, y, z;
	CameraData* data;
	GFC_Vector3D position;

	if (!self)
	{
		slog("The camera does not exist!");
		return;
	}

	data = (CameraData*)self->data;


	if (!data)
	{
		slog("Failed to find Camera's Data!");
		return NULL;
	}

	player = getPlayer();

	if (!player)
	{
		slog("Camera cannot find the player!");
		return;
	}

	data->cameraOffset = data->originalCameraOffset;
	
	view = gf3d_vgraphics_get_view_matrix();

	gfc_vector3d_add(data->cameraOffset, player->position, data->cameraOffset);
	gfc_matrix4_view(*view, data->cameraOffset, player->position, gfc_vector3d(0, 0, 1));
	


	
	//Camera Update?
	gfc_vector3d_add(self->position, self->position, self->velocity);

	gfc_vector3d_scale(data->cameraOffset, data->cameraOffset, data->cameraFollowDistance);
	gfc_vector3d_rotate_about_z(&data->cameraOffset, data->cameraRotation.x);

	self->rotation.z = data->cameraRotation.x;

	gfc_vector3d_rotate_about_x(&data->cameraOffset, data->cameraRotation.y);
	self->rotation.y = data->cameraRotation.y;

	/*
	
	
	view = gf3d_vgraphics_get_view_matrix();
	player = getPlayer();

	x = player->position.x + data->cameraFollowDistance;
	y = player->position.y + data->cameraFollowDistance;
	z = player->position.z + data->cameraFollowDistance;

	data->cameraRotation = player->rotation;
	
	
	

	gfc_vector3d_scale(offset, offset, cData->cameraFollowDistance);
	gfc_vector3d_rotate_about_z(&offset, cData->cameraRotation.x);

	self->rotation.z = cData->cameraRotation.x;

	gfc_vector3d_rotate_about_x(&offset, cData->cameraRotation.y);
	self->rotation.y = cData->cameraRotation.y;

	*/
	

	/*
	if (!getFirstPersonCam())
	{
		//Player rotation * X
		//cameraFollowDistance
		//				out mat		Camera position, position camera is looking,		what is considered up
		gfc_matrix4_view(*view, player->position, gfc_vector3d(x, y, z), gfc_vector3d(0, 0, 1));
	}
	else
	{
		gfc_matrix4_view(*view, gfc_vector3d(10, -20, 10), gfc_vector3d(x, y, z), gfc_vector3d(0, 0, 1));
		slog("Not first person camera");
	}

	*/


}

void cameraDraw(Entity* self)
{
	if (!self)
	{
		return;
	}
}

void cameraFree(Entity* self)
{
	if (!self || !self->data)
	{
		return;
	}

	CameraData* data = (CameraData*)self->data;

	free(data);
	self->data = NULL;


}
