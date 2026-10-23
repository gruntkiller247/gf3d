#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "entity.h"

typedef struct
{
	float cameraFollowDistance;
	GFC_Vector3D cameraOffset; //The distance from the camera and player
	GFC_Vector3D originalCameraOffset;
	GFC_Vector2D cameraRotation;
	GFC_Vector3D spawn;
}CameraData;

Entity* cameraEntityNew();

void cameraThink(Entity* self);

#endif