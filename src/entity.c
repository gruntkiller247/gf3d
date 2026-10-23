


#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_color.h"
#include "gfc_matrix.h"
#include "entity.h"
#include "model.h"


typedef struct
{
	Entity* entityList;
	Uint32 maxEnts;

}EntityManager;

static EntityManager entityManager = { 0 };

void entityClose();
void entityFree(Entity* thing);


void entityInit(Uint32 maxEnts)
{
	if (!maxEnts)
	{
		slog("Failed to initalize entiy system");
		return;
	}

	if (entityManager.maxEnts != 0)
	{
		slog("Cannot make a second entity manager!");
		return;
	}

	entityManager.entityList = gfc_allocate_array(sizeof(Entity), maxEnts);

	if (!entityManager.entityList)
	{
		slog("Failed to create entity list for %i entities!", maxEnts);
		return;
	}
	entityManager.maxEnts = maxEnts;

	slog("Entity System initalized!");
	atexit(entityClose);
	
}



Entity* entityNew()
{
	int c;

	for (c = 0; c < entityManager.maxEnts; c++)
	{
		if (entityManager.entityList[c]._inUse)
			continue;

		entityManager.entityList[c]._inUse = 1;
		entityManager.entityList[c].scale = gfc_vector3d(1,1,1);
		entityManager.entityList[c].color = GFC_COLOR_WHITE;
		//entityManager.entityList[c].model = "models/dino/dino.json"; //"models/floor/floor.json";
		entityManager.entityList[c].directionLooking = 0;
		return &entityManager.entityList[c];
	}
}

void entityUpdate(Entity* self)
{
	if (!self)
	{
		slog("Cannot update NULL entity!");
		return;
	}

	if (self->update)
	{
		self->update(self);
	}
}


void entityUpdateAll()
{
	int c;
	for (c = 0; c < entityManager.maxEnts; c++)
	{
		if (entityManager.entityList[c]._inUse)
			entityUpdate(&entityManager.entityList[c]);
	}
}

void entityThink(Entity* self)
{
	if (!self)
	{
		slog("Cannot think for a NULL entity!");
		return;
	}

	if (self->think)
	{
		self->think(self);
	}
}


void entityThinkAll()
{
	int c;
	for (c = 0; c < entityManager.maxEnts; c++)
	{
		if (entityManager.entityList[c]._inUse)
			entityThink(&entityManager.entityList[c]);
	}
}

void entityDraw(Entity* self)
{
	int c;

	if (!self || !self->_inUse)
	{
		slog("Cannot draw null/not in use entity!");
		return;
	}

	if (self->draw)
	{
		self->draw(self);
		
	}
	else
	{
		gfc_matrix4_from_vectors(self->mat, self->position, self->rotation, self->scale);

		gf3d_model_queue_render(self->model, self->mat, self->color);
	}

	//slog("I am being drawn and my name is %s",self->name);
	

}

void entityDrawAll()
{
	int c;

	for (c = 0; c < entityManager.maxEnts; c++)
	{
		if (!entityManager.entityList[c]._inUse)
			continue;

		entityDraw(&entityManager.entityList[c]);
	}
}


void entityFree(Entity* self)
{
	if (!self)
	{
		slog("Cannot free null entity!");
		return;
	}

	if (!self->_inUse)
	{
		return;
	}

	if (self->free)
	{
		self->free(self);
	}

	model_free(self->model);
	memset(self, 0, sizeof(Entity));
}


void entityClose()
{
	int c;

	if (!entityManager.entityList)
	{
		slog("Trying to free enetityManger without an entityList!");
		return;
	}

	for (c = 0; c < entityManager.maxEnts; c++)
	{
		if (entityManager.entityList[c]._inUse)
			entityFree(&entityManager.entityList[c]);
		
		
	}

	free(entityManager.entityList);
	memset(&entityManager, 0, sizeof(EntityManager));
	slog("Entity System Closed!");
}


void setPlayer(Entity* newPlayer)
{
	thePlayer = newPlayer;
}

Entity* getPlayer()
{
	return thePlayer;
}

void setCamera(Entity* newCam)
{
	theCamera = newCam;
}


Entity* getCamera()
{
	return theCamera;
}




