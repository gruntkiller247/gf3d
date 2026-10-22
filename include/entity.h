#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "model.h"
#include "gfc_color.h"



typedef struct Entity_S
{
	Uint8			_inUse;
	Model			*model;
	GFC_Matrix4		mat;
	

	GFC_Vector3D	position;
	GFC_Vector3D	rotation;
	GFC_Vector3D	scale;
	
	GFC_Vector3D	velocity;
	GFC_Color		color;
	
	void		(*think)(struct Entity_S* self);	
	void		(*update)(struct Entity_S* self);
	void		(*draw)(struct Entity_S* self);
	void		(*free)(struct Entity_S* self);		
	void*	data;								//Used for custom data. Must create a free function for it!

	GFC_TextLine	name;

}Entity;

static Entity* thePlayer = NULL;
static Uint8 firstPersonCam = 0;

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


void setPlayer(Entity* newPlayer);

Entity* getPlayer();

//Returns whether the first person camera is being used or not
int getFirstPersonCam();

void setFirstPersonCam(int in);
#endif