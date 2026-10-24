#ifndef __Wall_H__
#define __Wall_H__

#include "entity.h"

Entity* spawnWall(GFC_Vector3D position, Model* model);

void wallTouch(Entity* self, Entity* toucher);

#endif