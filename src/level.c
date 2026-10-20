#include "simple_logger.h"
#include "level.h"
#include "gfc_primitives.h"
#include "gf3d_mesh.h"
#include "gf3d_obj_load.h"

//Include jload.h

static Level* theLevel = NULL;

Level* levelNew()
{
	Level* level;
	level = gfc_allocate_array(sizeof(Level), 1);
	if (!level)
	{
		slog("Failed to allocate level data!");
	}

	//Boilerplate stuff
	gfc_matrix4_identity(level->terrainMatrix);

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
		slog("Failed to find skybox for level! Make sure to remove the fail if you do not have skyboxs implmented!");
		
		goto fail;
	}

	str = sj_object_get_string(data, "terrain");

	if (str)
	{
		//Do Terrain stuff!
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


//Keyhoe Broke
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
			t.a = prim->objData->faceVertices[prim->objData->outFace->verts[0]].vertex;
			t.b = prim->objData->faceVertices[prim->objData->outFace->verts[1]].vertex;
			t.c = prim->objData->faceVertices[prim->objData->outFace->verts[2]].vertex;
			
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