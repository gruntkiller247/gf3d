#include "monster.h"
#include "simple_logger.h"


typedef enum jsonArrayNum_S
{
	AN_TRASH = 0,

	AN_DEFAULT

}jsonArrayNum;

typedef struct
{
	int role;

}MonsterData;

void monsterThink(Entity* self);
void monsterFree(Entity* self);
void monsterUpdate(Entity* self);


Entity* monsterEntityNew(GFC_Vector2D position, int role)
{
	Entity* self;
	MonsterData* data;

	self = entityNew();

	if (!self)
	{
		slog("Failed to make Monster Entity!");
		return;
	}

	data = (MonsterData*)self->data;

	if (!data)
	{
		slog("Failed to make Monster Data!");
		return;
	}

	self->think = monsterThink;
	self->update = monsterUpdate;
	self->free = monsterFree;
	data->role = role;

	
}

void monsterThink(Entity* self)
{
	if (!self)
	{
		return;
	}
}

void monsterFree(Entity* self)
{
	if (!self)
	{
		return;
	}
}

void monsterUpdate(Entity* self)
{
	if (!self)
	{
		return;
	}
}

void monsterTouch(Entity* self, Entity* toucher)
{
	if (!self)
	{
		return;
	}
}

void loadMonster(Entity* self)
{
	if (!self)
	{
		slog("No Monster entity to load for monster!");
	}
}

