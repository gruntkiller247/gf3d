#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "model.h"
#include "gfc_color.h"

typedef enum
{
	LD_North = 0,
	LD_East,
	LD_South,
	LD_West,
	LD_ERROR = -1

}LookingDirections;

typedef enum
{
	TEAM_NONE,
	TEAM_PLAYER,
	TEAM_WALL,
	TEAM_ENEMY,
	TEAM_IGNORE,
	TEAM_ITEM
}Teams;


typedef struct Entity_S
{
	Uint8			_inUse;
	Model			*model;
	GFC_Matrix4		mat;
	Uint32			layer;
	Uint8			team;
	

	GFC_Vector3D	position;
	GFC_Vector3D	rotation;
	GFC_Vector3D	scale;
	
	GFC_Vector3D	velocity;
	GFC_Color		color;
	
	void		(*think)(struct Entity_S* self);	
	void		(*update)(struct Entity_S* self);
	void		(*draw)(struct Entity_S* self);
	void		(*free)(struct Entity_S* self);
	void		(*touch)(struct Entity_S* self, struct Entity_S* toucher);
	void*	data;								//Used for custom data. Must create a free function for it!

	GFC_TextLine	name;
	Uint8 directionLooking;

}Entity;

static Entity* thePlayer = NULL;
static Entity* theCamera = NULL;

/*
	@brief initialize the entity manager and pre-allocate a bunch 
	@param maxEnts how many entities can exist at any single moment
*/
void entityInit(Uint32 maxEnts);

/*
	@breif get a blank entity from the system
	@return NULL on error or out of memory, a valid pointer otherwise
*/
Entity * entityNew();

/*
	@brief call to clean up an entity after it is no longer needed
*/
void entityFree(Entity* self);

/*
	@brief run updates on all active entities
*/
void entityUpdateAll();

/*
	@briuef run thinks on all active entities
*/
void entityThinkAll();

/*
	@brief queue renders on all active entities
*/
void entityDrawAll();

void entityTouchAll();


void setPlayer(Entity* newPlayer);

Entity* getPlayer();

void setCamera(Entity* newCam);

Entity* getCamera();

void hitWall(Entity* toucher);

int checkLookingDirection(Entity* self);
#endif