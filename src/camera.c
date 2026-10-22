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

	data->cameraFollowDistance = 1;
	data->cameraRotation.x = 0;
	data->cameraRotation.y = GFC_PI / 6;

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
	

	view = gf3d_vgraphics_get_view_matrix();
	player = getPlayer();

	x = player->position.x + data->cameraFollowDistance;
	y = player->position.y + data->cameraFollowDistance;
	z = player->position.z + data->cameraFollowDistance;

	data->cameraRotation = player->rotation;
	
	if (!getFirstPersonCam())
	{
		//cameraFollowDistance
		//				out mat		Camera position, position camera is looking,		what is considered up
		gfc_matrix4_view(*view, player->position, gfc_vector3d(x, y, z), gfc_vector3d(0, 0, 1));
	}
	else
	{
		gfc_matrix4_view(*view, gfc_vector3d(10, -20, 10), gfc_vector3d(x, y, z), gfc_vector3d(0, 0, 1));
		slog("Not first person camera");
	}


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
