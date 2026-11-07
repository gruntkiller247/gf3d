#ifndef __BOMB_H__
#define __BOMB_H__

#include "entity.h"




Entity* bombEntityNew(GFC_Vector3D position, Uint8 team, Uint16 timeToLive, Uint16 role);

void bombThink(Entity* self);

void bombUpdate(Entity* self);

void bombFree(Entity* self);

void bombTouch(Entity* self, Entity* toucher);

#endif