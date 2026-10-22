#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "entity.h"


void monsterThink(Entity* self);

void monsterFree(Entity* self);

void monsterUpdate(Entity* self);

void monsterTouch(Entity* self, Entity* toucher);

Entity* monsterEntityNew(GFC_Vector2D position, int role);

#endif