#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"


Entity* spawnPlayer(GFC_Vector3D position);

void playerThink(Entity* self);
void playerUpdate(Entity* self);




#endif