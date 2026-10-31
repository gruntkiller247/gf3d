#include "simple_logger.h"
#include "level.h"
#include "gfc_primitives.h"
#include "gf3d_mesh.h"
#include "gf3d_obj_load.h"



static Level* theLevel = NULL;

void setLevel(Level* inLevel);

Level* levelNew()
{
	Level* level;
	level = gfc_allocate_array(sizeof(Level), 1);
	if (!level)
	{
		slog("Failed to allocate level data!");
	}

	//Boilerplate stuff

	//Read LevelLoad to get JSON stuff!
	level->minRoomSize = 3;
	level->maxRoomSize = 10;
	level->width = 100;
	level->height= 100;
	gfc_matrix4_identity(level->terrainMatrix);

	//Hard Coded bounds for now
	level->bounds = gfc_box(-10,-10,-10,10,10,10);

	setLevel(level);

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

	model_free(level->sky);
	model_free(level->terrain);

	Mix_HaltMusic();
	Mix_FreeMusic(level->backgroundMusic);

	sj_free(level->json);

	free(level);

}

void levelDraw(Level* level)
{
	GFC_Matrix4 id;
	if (!level)
	{
		return;
	}

	gfc_matrix4_identity(id);

	//modelqueuerender sky   level->sky,id,gfc_white
	//model queue render terrain   level->terrain, level->terrain matrix, ?
}


//Keyhoe Broke - might be working now?
Uint8 levelLineTest(Level* level, GFC_Edge3D edge, GFC_Vector3D *contactpoint)
{
	Mesh* mesh;
	MeshPrimitive *prim;
	ObjData *obj;
	int i, c, j;
	GFC_Triangle3D t;
	GFC_Vector3D out = { -1 };


	if (!level || !level->terrain || !level->terrain->mesh)
	{
		return -1;
	}

	c = gfc_list_count(level->terrain->mesh->primitives);

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
	int size;
	int* val;
	
	level = levelNew();

	if (!level)
	{
		slog("Failed to create the level!");
		return NULL;
	}

	//Level now knows its own X and Y size for where it wants to let the wander rng wander
	//Level now knows the min and max room size
	
	//Floor now needs to become a level map
	//Floor will be a list of lists based on the size of the level. size X * Y
	size = level->width * level->height;

	floor = gfc_allocate_array(sizeof(Uint16), level->width);
	

	if (!floor)
	{
		slog("Failed to create array for floorplan!");
		goto fail;
	}

	for (c = 0; c < floor->count; c++)
	{

	}
	

	return level;
	
	fail:

	if (level)
	{
		levelFree(level);
	}

	if (floor)
	{
		for (c = 0; c < floor->count; c++)
		{
			if (gfc_list_get_nth(floor,c))
			{
				gfc_list_delete_nth(floor,c);
			}
		}
		gfc_list_delete(floor);
	}
}




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