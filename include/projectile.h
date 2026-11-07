#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "entity.h"

//Projectiles do not set themselves inUse =0
//Must be done in the touch function of whatever they are touching!

Entity* projectileEntityNew(GFC_Vector3D position, Uint8 team, Uint16 timeToLive, Uint16 role);

void projectileThink(Entity* self);

void projectileUpdate(Entity* self);

void projectileFree(Entity* self);

void projectileTouch(Entity* self);

#endif