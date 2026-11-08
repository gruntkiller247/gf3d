#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "model.h"
#include "gfc_color.h"
#include "level.h"

typedef enum DS
{
	D_NONE = -1,
	D_NORTH = 0,
	D_NORTHEAST,
	D_EAST,
	D_SOUTHEAST,
	D_SOUTH,
	D_SOUTHWEST,
	D_WEST,
	D_NORTHWEST,
	D_MAX

}Directions;

typedef enum
{
	TTL_ZERO = 0,
	TTL_BOMB_DEFAULT = 50,
	TTL_BOMB_MELEE = 5,
	TTL_BOMB_LAVA = 50,
	TTL_PROJ_DEFAULT = 100,
	TTL_PROJ_MELEE = 5,

}TimeToLive;

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
	TEAM_ENEMY,
	TEAM_IGNORE,
	TEAM_ITEM,
	TEAM_TRAP
}Teams;

typedef enum role_s
{
	ROLE_ERROR = -1,
	ROLE_NONE,
	ROLE_DEAD,

	ROLE_PLAYER,

	ROLE_WALL,
	ROLE_FLOOR,
	ROLE_FLOOR_TRAP,

	ROLE_DEFAULT_PROJ,
	ROLE_DEFAULT_BOMB,
	ROLE_LAVA_BOMB,
	ROLE_FREEZE_PROJ,

	ROLE_TRASH,
	ROLE_SUICIDER,
	ROLE_STATION,
	ROLE_MOTHER,
	ROLE_FLY_SNIP,


}Role;

typedef enum status_time_s
{
	SE_FREEZE_TIME = 300,

}StatusEffectTimers;

typedef enum status_s
{
	SE_NONE = 0,
	SE_FREEZE = 1,

}StatusEffects;


typedef struct Entity_S
{
	Uint8			_inUse;
	Model* model;
	GFC_Matrix4		mat;
	Uint32			layer;
	Uint8			team;
	Uint32			id;
	Uint32			role;


	GFC_Vector3D	position;
	GFC_Vector3D	rotation;
	GFC_Vector3D	scale;

	GFC_Box			bounds;
	GFC_Vector3D	floorPoint;		//Point directly below me
	float			grounded;		//Got to level lind test add this here?

	GFC_Vector3D	velocity;
	GFC_Vector3D	velocityLastFrame;
	GFC_Color		color;

	void		(*think)(struct Entity_S* self);
	void		(*update)(struct Entity_S* self);
	void		(*draw)(struct Entity_S* self);
	void		(*free)(struct Entity_S* self);
	void		(*touch)(struct Entity_S* self, struct Entity_S* toucher);
	void* data;								//Used for custom data. Must create a free function for it!


	GFC_TextLine	name;
	Uint8 directionLooking;

	Uint8 hp;
	Uint8 maxHp;

	Uint8 isInvul;
	Uint8 damage;

	Uint8 isProj;

	Uint8 isFrozen;
	Uint8 frozenTimer;
	Uint8 maxFreeze;
	

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
Entity* entityNew();

/*
	@brief call to clean up an entity after it is no longer needed
*/
void entityFree(Entity* self);

/*
	Checks for not in use entities and calls entityFree on them!
*/
void entityFreeAll();

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

/*
	Returns a Vector3D with the difference between the entity's current and previous velocity

*/
GFC_Vector3D deltaVelocity(Entity* self);

//Probably need each entity type to define this
//void hitWall(Entity* toucher);

//int checkLookingDirection(Entity* self);

//void moveCheck(Entity* self, Entity* toucher);



#endif