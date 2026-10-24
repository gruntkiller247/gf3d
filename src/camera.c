#include "simple_logger.h"
#include "camera.h"
#include "gf3d_vgraphics.h"
#include "player.h"


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
	data->cameraOffset = gfc_vector3d(0, data->cameraFollowDistance, 0);
	data->originalCameraOffset = data->cameraOffset;
	

	self->data = data;

	setCamera(self);

	return self;
}

void cameraThink(Entity* self)
{
    GFC_Matrix4* view;
    Entity* player;
    PlayerData* pData;
    CameraData* data;
    GFC_Vector3D offset;
	GFC_Vector3D cameraPosition;

    if (!self || !self->data)
    {
        slog("The camera or its data does not exist!");
        return;
    }

    data = (CameraData*)self->data;

    player = getPlayer();

    if (!player || !player->data)
    {
        slog("Camera cannot find the player or player data!");
        return;
    }

    pData = (PlayerData*)player->data;

	
	float yaw = pData->playerRotation.x;
	float pitch = pData->playerRotation.y;
	float distance = data->cameraFollowDistance;

	//Uses spherical coordinates and trigonometric coordinate conversion
	//Calculate the camera's horizontal distance and height
	offset.x = distance * cosf(pitch) * sinf(yaw);
	offset.y = -distance * cosf(pitch) * cosf(yaw);
	offset.z = distance * sinf(pitch);

	//Place the camera relative to the player
	gfc_vector3d_add(cameraPosition,player->position,offset);
	

	/*
	//PlayerRotation.x is an angle need to make a vector
	//Same with Y

	offset.x = pData->playerRotation.x;
	offset.y = pData->playerRotation.y;
	offset.z = data->cameraFollowDistance;

	gfc_vector3d_add(offset, player->position, offset);
	*/

	view = gf3d_vgraphics_get_view_matrix();
	gfc_matrix4_view(*view, cameraPosition, player->position, gfc_vector3d(0, 0, 1));
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
