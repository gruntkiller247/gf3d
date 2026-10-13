#include "simple_logger.h"
#include "player.h"
#include "gfc_vector.h"
#include "entity.h"
#include "model.h"

Entity* spawnPlayer(GFC_Vector3D position)
{
	Entity* self;
	self = entityNew();
	if (!self)
	{
		
		slog("Failed to make the player");
		return NULL;
	}

	self->model = model_load("models/dino/dino.model");
	self->position = position;
	self->think = playerThink;
	
	return self;
}