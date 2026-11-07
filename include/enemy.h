#ifndef __Enemy_H__
#define __Enemy_H__

#include "entity.h"
#include "model.h"

Entity* enemyEntityNew(GFC_Vector3D pos, int role);

void enemyThink(Entity* self);

void enemyUpdate(Entity* self);

void enemyFree(Entity* self);

void enemyTouch(Entity* self, Entity* toucher);

void enemyMove(Entity* self);

void enemyDraw(Entity* self);


#endif