#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "entity.h"

typedef struct
{
	float cameraFollowDistance;
	GFC_Vector3D cameraRotation;
	GFC_Vector3D spawn;
}CameraData;

Entity* cameraEntityNew();

void cameraThink(Entity* self);

#endif