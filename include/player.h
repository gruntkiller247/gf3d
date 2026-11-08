#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

typedef struct Player_S
{
	Entity* camera;
	GFC_Vector2D playerRotation;
	float panSpeed;

}PlayerData;

typedef enum
{
	PLAYER_JUMP_HEIGHT = 10,

}PlayerStats;

Entity* playerEntityNew(GFC_Vector3D position);

void playerThink(Entity* self);

void playerUpdate(Entity* self);

void playerFree(Entity* self);

void playerTouch(Entity* self, Entity* toucher);

void playerDraw(Entity* self);



#endif