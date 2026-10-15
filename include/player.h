#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"


Entity* spawnPlayer(GFC_Vector3D position);

int playerThink(Entity* self);
int playerUpdate(Entity* self);




#endif