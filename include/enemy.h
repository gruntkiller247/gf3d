#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "entity.h"
#include "model.h"

Entity* enemyEntityNew(GFC_Vector3D pos, Uint8 role);

void enemyThink(Entity* self);

void enemyUpdate(Entity* self);

void enemyFree(Entity* self);

void enemyTouch(Entity* self, Entity* toucher);

void enemyTakeDamage(Entity* self, Entity* toucher);

void enemyMove(Entity* self);

void enemyDraw(Entity* self);


#endif