#include "simple_logger.h"
#include "level.h"
#include "gfc_primitives.h"
#include "gf3d_mesh.h"
#include "gf3d_obj_load.h"
#include "wall.h"
#include "tile.h"


static Level* theLevel = NULL;

Level* levelLoadTest();

void setLevel(Level* inLevel);

Level* levelNew()
{
	
	Level* level;
	/*
	level = gfc_allocate_array(sizeof(Level), 1);
	if (!level)
	{
		slog("Failed to allocate level data!");
	}
	*/

	level = malloc(sizeof(Level));

	if (!level)
	{
		slog("Someting went wrong!");
		return;
	}


	//Boilerplate stuff

	//Read LevelLoad to get JSON stuff!
	level->minRoomSize = 3;
	level->maxRoomSize = 10;

	//gfc_matrix4_identity(level->terrainMatrix);

	//Hard Coded bounds for now
	level->bounds = gfc_box(-10,-10,-10,300,300,20);
	
	level->backgroundMusic = NULL;

	setLevel(level);

	//DO THING HERE!

	return level;
}

Level* levelLoad(const char* filename)
{
	SJson *json;
	SJson* data;
	const char* str;
	Level* level;
	level = levelNew();

	if (!level)
	{
		slog("Failed to create a new level!");
		return;
	}

	json = sj_load(filename);

	if (!json)
	{
		slog("Failed to load JSON!");
		goto fail;
	}

	level->json = json;

	data = sj_object_get_value(json,"level");

	/*
	str = sj_object_get_string(data, "sky");

	if (str)
	{
		level->sky = model_load(str);

		if (level->sky)
		{
			gfc_line_cpy(level->skyFile, str);
			slog("Level skybox loaded!");
		}
		else
		{
			slog("Failed to find Level Skybox!");
		}
	}
	else
	{
		slog("Failed to find skybox for level! Make sure to add the fail if you do not have skyboxs implmented!");
		
		//goto fail;
	}
	*/

	str = sj_object_get_string(data, "terrain");

	if (str)
	{
		//Do Terrain stuff!
		//Drunkard walking!
	}

	str = sj_object_get_string(data, "music");

	if (str)
	{
		//Do music stuff!
		//gfc_sound(load_music);
		//Mix_PlayMusic(the music thing)
	}
	



	theLevel = level;
	return level;


	fail:
	//FREE THINGS HERE! CHECK IF EXIST!
	if (json)
	{
		sj_free(json);
	}

	if (level)
	{
		levelFree(level);
	}
	//Do JSON STUFF HERE!
}

void levelFree(Level* level)
{
	if (!level)
	{
		return;
	}

	//model_free(level->sky);
	//model_free(level->terrain);

	if (level->floorPlan)
	{
		slog("Floor plan needs to free tiles!");
		free(level->floorPlan);
	}

	
	if (level->backgroundMusic)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(level->backgroundMusic);

	}

	sj_free(level->json);

	free(level);

}

void levelDraw(Level* level)
{
	int c = 0;
	int x, y;
	int z = 0;
	GFC_Matrix4 mat;
	
	
	//Model* test;

	//test = model_load("models/Cube/Cube.json");

	if (!level)
	{
		return;
	}

	gfc_matrix4_identity(mat);
	
	//slog("sizeMax: %i", level->sizeMax);
	
	for (c = 0; c < level->sizeMax; c++)
	{

		x = c % level->width;
		y = c / level->width;

		//slog("\nX pos: %i\nY pos: %i\nZ pos: %i",x,y,z);
		
		//Vector3 = x,y,z
		//At some point need to add the size of the bounds

		z += 1;

		//Switch statement to see what tile type to draw later
		//level->floorPlan->elements[c];
		

		//gfc_matrix4_make_translation(mat,*temp);

		//spawnWall(gfc_vector3d(x * 2, y * 2, z) , level->floor);
		//gf3d_model_queue_render(level->floor, mat ,GFC_COLOR_WHITE);
	}

	//modelqueuerender sky   level->sky,id,gfc_white
	//model queue render terrain   level->terrain, level->terrain matrix, ?
}


/*Keyhoe Broke - might be working now ?
Uint8 levelLineTest(Level* level, GFC_Edge3D edge, GFC_Vector3D *contactpoint)
{
	Mesh* mesh;
	MeshPrimitive *prim;
	ObjData *obj;
	int i, c, j;
	GFC_Triangle3D t;
	GFC_Vector3D out = { -1 };


	if (!level)
	{
		return -1;
	}

	/*c = gfc_list_count(level->terrain->mesh->primitives);

	for (i = 0; i < c; i++)
	{
		prim = gfc_list_nth(level->terrain->mesh->primitives,i);

		if (!prim)
		{
			continue;
		}
		if (!prim->objData)
		{
			continue;
		}


		
		for (j = 0; j < prim->objData->face_count; j++)
		{
			t.a = prim->objData->faceVertices[prim->objData->outFace[j].verts[0]].vertex;
			t.b = prim->objData->faceVertices[prim->objData->outFace[j].verts[1]].vertex;
			t.c = prim->objData->faceVertices[prim->objData->outFace[j].verts[2]].vertex;
			
			if (gfc_triangle_edge_test(edge, t, &out))
			{
				
				if (contactpoint)
				{
					*contactpoint = out;
				}
					
				return 1;
			}
		}
	}
	

	//Failed to hit anything
	return 0;
	
}

*/


Level* getLevel()
{
	return theLevel;
}

Level* levelLoadTest()
{
	Level* level;
	GFC_List* floor;
	GFC_List* row;
	int c, d;
	int x = 0, y = 0;
	int z = -10;
	int sizeMax, size, widthLevel, heightLevel;
	int rdNum;

	Entity* tempTile;
	
	level = levelNew();

	if (!level)
	{
		slog("Failed to create the level!");
		return NULL;
	}
	
	//2D -> 1D
	//int index = y * width + x;

	//1D -> 2D
	//int x = index % width;
	//int y = index / width;
	
	int width = 20, height = 20;
	
	sizeMax = width * height;

	level->sizeMax = sizeMax;
	level->width = width;
	level->height = height;

	level->minRoomSize = 5;
	level->maxRoomSize = 5;
	
	level->floorPlan = gfc_list_new_size(sizeMax);

	if (!level->floorPlan)
	{
		slog("Failed to create array for floorplan!");
		goto fail;
	}

	for (c = 0; c < sizeMax; c++)
	{
		rdNum = rand() % (10 - 1 + 1) + 1;
		//rdNum = 0;

		if (rdNum == 1)
		{
			//slog("FREEZING!");
			//tempTile = tileEntityNew(TT_FREEZE, gfc_vector3d(x, y, z));
			tempTile = tileEntityNew(TT_FLOOR, gfc_vector3d(x, y, z));
		}
		else if (rdNum == 2)
		{
			tempTile = tileEntityNew(TT_ENEMY_SPAWNER, gfc_vector3d(x, y, z));
		}
		else
		{
			tempTile = tileEntityNew(TT_FLOOR, gfc_vector3d(x, y, z));
		}


		

		if (!tempTile)
		{
			slog("Failed tempTile");
			goto fail;
		}

		x += tempTile->bounds.w;

		
		if (c != 0 && (c+1) % width == 0)
		{
			x = 0;
			y += tempTile->bounds.h;
		}


		//y = tempTile->bounds.y + tempTile->bounds.h;

		//slog("\nX pos: %i\nY pos: %i\nZ pos: %i", x, y, z);
		//Create each entity based on the json map read/RNG. For now everything is just default tile
		gfc_list_append(level->floorPlan, tempTile);
		//slog("X: %i", x);
		
		
	}

	slog("Floorplan created.\nSize: %i\nWidth: %i\nHeight: %i",sizeMax,level->width,level->height);
	
	//Load the floorplan
	

	return level;
	
fail:

	if (level->floorPlan)
	{
		free(floor);
	}

	if (level)
	{
		levelFree(level);
	}


}

//Also pushes back TILES! DO NOT FORGET AGAIN!
void levelPosCheck(Level* level, Entity* self)
{
	if (!level)
	{
		slog("Illegal Level!");
		return;
	}

	if (self->position.x < level->bounds.x)
	{
		//Player is out of bounds on the min X
		//Set player's pos to the bounds.x
		self->position.x = level->bounds.x;
	}

	if (self->position.y < level->bounds.y)
	{
		self->position.y = level->bounds.y;
	}

	if (self->position.z < level->bounds.z)
	{
		self->position.z = level->bounds.z;
	}

	if (self->position.x > level->bounds.x + level->bounds.w)
	{
		self->position.x = level->bounds.x + level->bounds.w;
	}

	if (self->position.y > level->bounds.y + level->bounds.h)
	{
		self->position.y = level->bounds.y + level->bounds.h;
	}

	if (self->position.z > level->bounds.z + level->bounds.d)
	{
		self->position.z = level->bounds.z + level->bounds.d;
	}
}

/*
	Check if the bounding box provided is clipping or outside the level bounds
	Recieves the level to compare and the bounding box in world space
	Returns ENUMS 0-6 to tell you which axis and either min or max is out of bounds
	Returns -1 if the level is illegal!
	
*/
Uint8 levelBoundsCheck(Level* level, GFC_Box aabb)
{
	if (!level)
	{
		slog("Cannot bounds check illegal level");
		return -1;
	}

	if (aabb.x < level->bounds.x)
	{
		return LB_MIN_X;
	}

	if (aabb.y < level->bounds.y)
	{
		return LB_MIN_Y;
	}

	if (aabb.z < level->bounds.z)
	{
		return LB_MIN_Z;
	}

	if (aabb.x + aabb.w > level->bounds.x + level->bounds.w)
	{
		return LB_MAX_X;
	}

	if (aabb.y + aabb.h > level->bounds.y + level->bounds.h)
	{
		return LB_MAX_Y;
	}

	if (aabb.z + aabb.d > level->bounds.z + level->bounds.d)
	{
		return LB_MAX_Z;
	}



	return LB_NO_TOUCH;
}

void setLevel(Level* inLevel)
{
	theLevel = inLevel;
}



Uint32 levelGetTileIndex(Level* level, Uint16 x, Uint16 y)
{
	if (!level)
	{
		return -1;
	}

	//return y * level->tileSize.x
}