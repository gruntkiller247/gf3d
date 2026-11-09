#include "simple_logger.h"
#include "enemy.h"
#include "projectile.h"

typedef enum
{
	MS_ERROR = -1,
	MS_IDLE = 0,
	MS_MOVE,
	MS_FLY,
	MS_SHOOT,

}MonsterState;

typedef struct Enemy_S
{
	Uint8 lastDirection;
	Uint8 currentDirection;

	Uint8 moveTimer;
	Uint8 moveMax;

	GFC_Vector2D enemyRotation;

	GFC_Vector3D lookAt;

	Uint8 motherTimer;
	Uint16 motherMax;
	Uint8 state;

	Uint8 shootTimer;
	Uint8 shootMax;

	//Uint8 maxFlightHeight;  //Z axis
	GFC_Vector3D maxDistFromPlayer;

}EnemyData;

void enemyThink(Entity* self);
void enemyUpdate(Entity* self);
void enemyFree(Entity* self);
void enemyTouch(Entity* self, Entity* toucher);
void moveRandom(Entity* self, Uint8 direction);
GFC_Vector3D move(Entity* self, Uint8 direction, GFC_Vector3D move);


Entity* enemyEntityNew(GFC_Vector3D pos, Uint8 role)
{
	Entity* self;
	EnemyData* data;

	self = entityNew();

	if (!self)
	{
		slog("Failed to make enemy!");
		return NULL;
	}

	gfc_vector3d_copy(self->position, pos);

	self->role = role;
	self->team = TEAM_ENEMY;

	self->think = enemyThink;
	self->touch = enemyTouch;
	self->update = enemyUpdate;
	self->free = enemyFree;


	self->data = malloc(sizeof(EnemyData));

	data = (EnemyData*)self->data;

	if (!data)
	{
		slog("Failed to make Player Data!");
		return NULL;
	}


	data->lastDirection = rand() % D_MAX;
	data->currentDirection = rand() % D_MAX;

	data->moveTimer = 0;
	data->moveMax = 100;

	data->enemyRotation.x = 0;
	data->enemyRotation.y = GFC_PI / 6;

	


	switch (role)
	{
		case ROLE_TRASH:

			self->model = model_load("models/dino/dino.json");
			self->color = GFC_COLOR_RED;
			self->maxHp = 1;
			self->hp = self->maxHp;
			self->damage = 1;
			
			break;

		case ROLE_SUICIDER:
			self->model = model_load("models/Cube/Cube.json");//model_load("models/dino/dino.model");
			self->color = GFC_COLOR_ORANGE;
			self->maxHp = 1;
			self->hp = self->maxHp;
			self->damage = 1;

			data->moveMax = 10;
			data->lookAt = getPlayer()->position;

			break;

		case ROLE_STATION:
			self->model = model_load("models/Cube/Cube.json");//model_load("models/dino/dino.model");
			self->color = GFC_COLOR_YELLOW;
			self->maxHp = 1;
			self->hp = self->maxHp;
			self->damage = 1;

			data->shootMax = 10;
			data->lookAt = getPlayer()->position;


			break;

		case ROLE_MOTHER:
			self->model = model_load("models/dino/dino.json");
			self->color = GFC_COLOR_GREEN;
			self->maxHp = 1;
			self->hp = self->maxHp;
			self->damage = 1;

			data->motherMax = 200;

			break;

		case ROLE_FLY_SNIP:
			self->model = model_load("models/dino/dino.json");//model_load("models/dino/dino.model");
			self->color = GFC_COLOR_BLUE;
			self->maxHp = 1;
			self->hp = self->maxHp;
			self->damage = 1;

			data->shootMax = 10;
			data->maxDistFromPlayer = gfc_vector3d(10,10,10);

			//data->maxFlightHeight = 20;
			data->state = MS_FLY;
			data->moveMax = 10;
			data->lookAt = getPlayer()->position;
			break;

		case ROLE_QUAKE_ENEMY:
			self->model = model_load("models/quake/quake.json");
			self->maxHp = 1;
			self->hp = self->maxHp;
			self->damage = 0;
			
			Uint8 temp = 5;
			self->scale.x = temp;
			self->scale.y = temp;
			self->scale.z = temp;

			break;
		
		default:

			slog("Default Enemy role!");
			self->model = model_load("models/dino/dino.json");

			break;
	}

	self->bounds = self->model->mesh->bounds;

	return self;
}

void enemyThink(Entity* self)
{
	EnemyData* data;

	if (!self)
	{
		return;
	}

	data = (EnemyData*)self->data;

	if (!data)
	{
		return;
	}

	if (self->hp <= 0)
	{
		self->role = ROLE_DEAD;
	}

	data->moveTimer++;
	data->shootTimer++;

	switch (self->role)
	{
		case ROLE_MOTHER:
			//Same movement as trash
			data->motherTimer++;
		case ROLE_TRASH:

			if (data->moveTimer >= data->moveMax)
			{
				data->moveTimer = 0;

				switch (self->role)
				{

					default:
						switch (data->lastDirection)
						{
						case D_NORTH:

							moveRandom(self, D_NORTH);

							break;

						case D_NORTHEAST:
							moveRandom(self, D_NORTHEAST);
							break;

						case D_EAST:
							moveRandom(self, D_EAST);
							break;

						case D_SOUTHEAST:
							moveRandom(self, D_SOUTHEAST);
							break;

						case D_SOUTH:
							moveRandom(self, D_SOUTH);
							break;

						case D_SOUTHWEST:
							moveRandom(self, D_SOUTHWEST);
							break;

						case D_WEST:
							moveRandom(self, D_WEST);
							break;

						case D_NORTHWEST:
							moveRandom(self, D_NORTHWEST);
							break;

						default:
							slog("Monster does not have Last Direction!");
						}
				}
			}

			break;
		case ROLE_FLY_SNIP:


			if (self->position.z >= data->maxDistFromPlayer.z)
			{
				//State should be to shoot
				data->state = MS_SHOOT;
			}
			else
			{
				data->state = MS_FLY;
			}

			if (data->moveTimer >= data->moveMax)
			{
				//data->moveTimer = 0;

				switch (self->role)
				{

				default:
					switch (data->lastDirection)
					{
					case D_NORTH:

						moveRandom(self, D_NORTH);

						break;

					case D_NORTHEAST:
						moveRandom(self, D_NORTHEAST);
						break;

					case D_EAST:
						moveRandom(self, D_EAST);
						break;

					case D_SOUTHEAST:
						moveRandom(self, D_SOUTHEAST);
						break;

					case D_SOUTH:
						moveRandom(self, D_SOUTH);
						break;

					case D_SOUTHWEST:
						moveRandom(self, D_SOUTHWEST);
						break;

					case D_WEST:
						moveRandom(self, D_WEST);
						break;

					case D_NORTHWEST:
						moveRandom(self, D_NORTHWEST);
						break;

					default:
						slog("Monster does not have Last Direction!");
					}
				}
			}

		case ROLE_QUAKE_ENEMY:

			break;


		case ROLE_SUICIDER:
			//data->lookAt = getPlayer()->position;

			

		case ROLE_STATION:
			data->lookAt = getPlayer()->position;
			//slog("\nWhere I think the player is!\nX: %f\nY: %f", data->lookAt.x, data->lookAt.y);
			break;

		case ROLE_DEAD:
			self->_inUse = 0;
			break;



		default:

			slog("Enemy has no role!");
			break;
	}


	
}

void enemyUpdate(Entity* self)
{
	EnemyData* data;
	GFC_Vector3D moveV = { 0 };
	GFC_Vector3D offset;
	GFC_Vector3D temp = { 0 };
	Entity* proj;
	int rdNum;
	
	if (!self || !self->_inUse)
	{
		return;
	}
	

	data = (EnemyData*)self->data;

	if (!data)
	{
		return;
	}

	switch (self->role)
	{
		case ROLE_MOTHER:
			//Same movement as trash

			if (data->motherTimer >= data->motherMax)
			{
				data->motherTimer = 0;
				enemyEntityNew(self->position,ROLE_TRASH);
			}

		case ROLE_TRASH:
			moveV = move(self, data->currentDirection, moveV);
			break;

		case ROLE_SUICIDER:
			if (data->moveTimer >= data->moveMax)
			{
				data->moveTimer = 0;
				GFC_Vector3D temp = self->position;

				if (temp.x > data->lookAt.x)
				{
					moveV.x = -1;
				}
				else
				{
					moveV.x = 1;
				}

				if (temp.y > data->lookAt.y)
				{
					moveV.y = -1;
				}
				else
				{
					moveV.y = 1;
				}


			}

			break;

		case ROLE_STATION:
			//Spawn a projectile and have it move in a direction towards the player!

			if (data->shootTimer >= data->shootMax)
			{
				data->shootTimer = 0;
				proj = projectileEntityNew(self->position, self->team, TTL_PROJ_DEFAULT, ROLE_DEFAULT_PROJ);
				
				if (proj)
				{
					gfc_vector3d_sub(temp,getPlayer()->position,self->position);


					//slog("\nWhere I think the player is!\nX: %f\nY: %f", temp.x, temp.y);
					projectileGiveMove(proj,temp);
				}
			}

		case ROLE_FLY_SNIP:

			switch (data->state)
			{
				case MS_FLY:
					//slog("I AM FLYING!");
					moveV = move(self, data->currentDirection, moveV);
					
					if (data->moveTimer >= data->moveMax)
					{
						//slog("I AM FLYING AND MOVING!");
						
						data->moveTimer = 0;
						GFC_Vector3D temp = self->position;

						
						if (temp.z <= data->maxDistFromPlayer.z)
						{
							moveV.z = 1;
						}
						else
						{
							moveV.z = 0;
						}
						//TODO FIGURE OUT WHY THIS NOT WORKING!
						//moveV = move(self, data->currentDirection, moveV);

					}
					break;

				case MS_SHOOT:
					if (data->shootTimer >= data->shootMax)
					{
						moveV = move(self, data->currentDirection, moveV);
						moveV.z -= self->position.z;
						

						data->shootTimer = 0;
						proj = projectileEntityNew(self->position, self->team, TTL_PROJ_DEFAULT, ROLE_DEFAULT_PROJ);

						if (proj)
						{
							gfc_vector3d_sub(temp, getPlayer()->position, self->position);


							//slog("\nWhere I think the player is!\nX: %f\nY: %f", temp.x, temp.y);
							projectileGiveMove(proj, temp);
						}
					}
					break;

				default:
					slog("I am some other state!");
					break;
			}



			break;

		case ROLE_QUAKE_ENEMY:

			break;

		default:
			slog("NO MOVEMENT ROLE CHECK!");
			break;
	}


	//slog("Move X: %f\nMove Y: %f ", moveV.x, moveV.y);
	
	if ((moveV.x) || (moveV.y))
	{
		if (!self->isFrozen)
		{
			gfc_vector3d_normalize(&moveV);

			gfc_vector3d_rotate_about_z(&moveV, data->enemyRotation.x);

			self->velocity = moveV;
		}

	}

	if (moveV.z)
	{
		if (moveV.z > 0)
		{
			rdNum = rand() % (10 - 1 + 1) + 1;
			if (rdNum > 5)
			{
				self->position.z += MONSTER_JUMP_HEIGHT;
			}
			else
			{
				self->position.z += MONSTER_JUMP_HEIGHT/2;
			}
			
		}
		else
		{
			//self->position.z--;
		}
	}


	gfc_vector3d_add(self->position, self->position, self->velocity);
	offset = gfc_vector3d(0, 0, 100);
	

	gfc_vector3d_rotate_about_z(&offset, data->enemyRotation.x);
	self->rotation.z = data->enemyRotation.x;
	
	gfc_vector3d_rotate_about_x(&offset, data->enemyRotation.y);
	self->rotation.y = data->enemyRotation.y;

	if (gfc_vector3d_magnitude_squared(self->velocity) > 0.1)
	{
		gfc_vector3d_scale(self->velocity, self->velocity, 0.8);
	}
	else
	{
		//slog("Clearing!");
		gfc_vector3d_clear(self->velocity);
	}



	
	
}

void enemyFree(Entity* self)
{
	if (!self)
	{
		return;
	}

	if (self->data)
	{
		free(self->data);
		self->data = NULL;
	}
}

void enemyTouch(Entity* self, Entity* toucher)
{
	Uint8 statusEffect;

	if (!self || !toucher)
	{
		//slog("AGKNAJSGNASF");
		return;
	}

	//Player's box
	//Min num
	float pX, pY, pZ;
	float pW, pH, pD;
	//Max num are the aboves combined

	//Toucher's box
	//Min nums
	float tX, tY, tZ;
	float tW, tH, tD;
	//Max num are the aboves combined

	pX = self->bounds.x + self->position.x;
	pY = self->bounds.y + self->position.y;
	pZ = self->bounds.z + self->position.z;
	pW = self->bounds.w;
	pH = self->bounds.h;
	pD = self->bounds.d;

	tX = toucher->bounds.x + toucher->position.x;
	tY = toucher->bounds.y + toucher->position.y;
	tZ = toucher->bounds.z + toucher->position.z;
	tW = toucher->bounds.w;
	tH = toucher->bounds.h;
	tD = toucher->bounds.d;

	
	//Check if we are touching
	if (pX <= tX + tW && pX + pW >= tX && pY <= tY + tH && pY + pH >= tY && pZ <= tZ + tD && pZ + pD >= tZ)
	{
		//slog("TOUCHER ROLE: %i", toucher->role);
		if (toucher->team == TEAM_PLAYER)
		{
			//slog("ENEMY TOUCHED by player!");

			self->hp-= toucher->damage;

			if (toucher->isProj)
			{
				statusEffect = projectileCanStatus(toucher);
				switch (statusEffect)
				{
					case SE_FREEZE:
						self->isFrozen = 1;

						break;

					case SE_GRAVITY:

						self->reverseGravity = 1;

					default:

						break;
				}
				toucher->_inUse = 0;
				
			}
			return;
		}


		//Calculate overlap on each axis
		float overlapX = (pX + pW) - tX;
		float overlapY = (pY + pH) - tY;
		float overlapZ = (pZ + pD) - tZ;

		//Also calculate overlap from the opposite direction
		float overlapX2 = (tX + tW) - pX;
		float overlapY2 = (tY + tH) - pY;
		float overlapZ2 = (tZ + tD) - pZ;

		if (overlapX > overlapX2)
		{
			overlapX = overlapX2;
		}

		if (overlapY > overlapY2)
		{
			overlapY = overlapY2;
		}

		if (overlapZ > overlapZ2)
		{
			overlapZ = overlapZ2;
		}

		if (overlapX <= overlapY && overlapX <= overlapZ)
		{
			//x collision
			if (pX < tX)
				self->position.x -= overlapX;
			else
				self->position.x += overlapX;
		}
		else if (overlapY <= overlapX && overlapY <= overlapZ)
		{
			//y collision
			if (pY < tY)
				self->position.y -= overlapY;
			else
				self->position.y += overlapY;
		}
		else
		{
			
			//z collision
			if (pZ < tZ)
				self->position.z -= overlapZ;
			else
				self->position.z += overlapZ;
			
		}


	}
	else
	{
		//Not touching on the X
	}
}

/*
	Determins what direction the monster should be moving in
	Called in monnster Think!
*/
void moveRandom(Entity* self, Uint8 direction)
{
	if (!self)
		return;

	int num, temp, directionNew;
	num = (rand() % 120) + 1;


	if (num <= 10)
	{
		//1-10 
		//10% chance to do a 180
		directionNew = direction + 4;



	}
	else if (num <= 30)
	{
		//11-40
		//30% chance to move back right or back left
		temp = rand() % 2;
		if (temp)
		{
			//Back Right
			directionNew = direction + 3;

		}
		else
		{
			//Back Left
			directionNew = direction - 3;
		}

	}
	else if (num <= 60)
	{
		//Left or Right
		temp = rand() % 2;
		if (temp)
		{
			//Right
			directionNew = direction + 2;

		}
		else
		{
			//Left
			directionNew = direction - 2;
		}

	}
	else if (num <= 100)
	{
		//Move front right or front left
		temp = rand() % 2;
		if (temp)
		{
			//Front Right
			directionNew = direction + 1;

		}
		else
		{
			//Front Left
			directionNew = direction + 1;
		}
	}
	else
	{
		//Move Straight
		directionNew = direction;
	}

	if (directionNew < 0)
		directionNew += D_MAX;
	else if (directionNew > D_MAX - 1)
		directionNew -= D_MAX;



	((EnemyData*)self->data)->currentDirection = directionNew;
}

/*
	Adds the enemy's movement instructions to the move vector
	Called in monster Update
*/
GFC_Vector3D move(Entity* self, Uint8 direction, GFC_Vector3D move)
{
	EnemyData* data;

	if (!self)
	{
		return gfc_vector3d(0, 0, 0);
	}


	data = (EnemyData*)self->data;

	if (!data)
	{
		return gfc_vector3d(0, 0, 0);
	}


	//slog("Monster is trying to move! Direction is %i",direction);

	switch (direction)
	{
	case(D_NORTH):
		
		//slog("Moving North!");
		move.x = 1;
		data->enemyRotation.x = 0;
		break;

	case(D_NORTHEAST):
		move.x = 1;
		move.y = 1;
		data->enemyRotation.x = GFC_PI/4;
		//slog("Moving North East!");
		break;

	case(D_EAST):
		move.y = 1;
		data->enemyRotation.x = GFC_PI / 2;
		//slog("Moving East!");
		break;

	case(D_SOUTHEAST):
		move.x = -1;
		move.y = 1;
		data->enemyRotation.x = 3 * GFC_PI / 4;
		//slog("Moving South East!");
		break;

	case(D_SOUTH):
		move.x = -1;
		//slog("Moving South");
		break;

	case(D_SOUTHWEST):
		move.x = -1;
		move.y = -1;
		data->enemyRotation.x = 5*GFC_PI/4;
		//slog("Moving South West!");
		break;

	case(D_WEST):
		move.y = -1;
		data->enemyRotation.x = 3 * GFC_PI / 2;
		//slog("Moving West!");
		break;

	case(D_NORTHWEST):
		move.x = 1;
		move.y = -1;
		data->enemyRotation.x = 7 * GFC_PI / 4;
		//slog("Moving North West!");
		break;

	default:

		slog("No real direction given for monster movement!");
		return;


	}
	//slog("Move X: %f\nMove Y: %f ", move.x, move.y);
	
	return move;
}