#include "model.h"
#include "gf3d_buffers.h"
#include "gf3d_swapchain.h"
#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_commands.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gf3d_mesh.h"

typedef struct
{
	Model* modelList;
	Uint32 modelCount;
	Pipeline *pipe;
	VkDevice device;
	Texture* defaultTexture;

}ModelManager;

//functions to make
//movel_render
//model render generic
//model close - use model delete
//gf3d_texture_free(model_manager.defaultTexture
void model_delete(Model* model);
void model_close();

static ModelManager modelManager = { 0 };

void model_init_system(Uint32 modelCount)
{
	Uint32 count;
	
	if (!modelCount)
	{
		slog("Can't create model manager with 0");
		return;
	}

	if (modelManager.modelCount != 0)
	{
		slog("Cannot intialize model system twice!");
		return;
	}
	
	modelManager.modelList = (Model*)gfc_allocate_array(sizeof(Model), modelCount);

	if (!modelManager.modelList)
	{
		slog("Failed to make modelList in model manager!");
		return;
	}

	gf3d_mesh_init(1024);

	modelManager.device = gf3d_vgraphics_get_default_logical_device();

	gf3d_mesh_get_attribute_descriptions(&count);
	modelManager.pipe = gf3d_pipeline_create_from_config(
		gf3d_vgraphics_get_default_logical_device(),
		"config/model_pipeline.cfg",
		gf3d_vgraphics_get_view_extent(),
		modelCount,
		gf2d_sprite_get_bind_description(),
		gf2d_sprite_get_attribute_descriptions(NULL),
		count,
		sizeof(ModelUBO),
		VK_INDEX_TYPE_UINT16
	);

	modelManager.defaultTexture = gf3d_texture_load("images/default.png");

	if (!modelManager.defaultTexture)
	{
		slog("DEFAULT TEXTURE DOES NOT EXIST!");
		return;
	}

	atexit(model_close);
	
}

void model_close()
{
	int i;
	gf3d_texture_free(modelManager.defaultTexture);

	//Do a loop to clear everything

	for (i = 0; i < modelManager.modelCount; i++)
	{
		model_free(&modelManager.modelList[i]);
	}

	//Did I miss something?

}

void model_free(Model* model)
{
	if (!model)
		return;

	model->_refCount--;

	if (!model->_refCount == 0)
	{
		model_delete(model);
	}
}

void model_delete(Model* model)
{
	if (!model)
		return;

	gf3d_texture_free(model->texture);
	gf3d_mesh_free(model->mesh);


	memset(model, 0, sizeof(Model));
}

//Model new
//Simliar to mesh?

//I think this is correct
Model* modelNew()
{
	//Simliar to mesh, no primitives
	//If no ref count && no filename set ref count to 1 and return a pointer
	//Otherwise if ref count is 0 and there is a file name, delete that part in memory in the array

	int c;
	int foundEmptyIndex;
	int emptyIndex;

	for (c = 0; c < modelManager.modelCount; c++)
	{
		if(modelManager.modelList[c]._refCount != 0)
			continue;

		if (emptyIndex < 0)
			emptyIndex = c;

		if (modelManager.modelList[c]._refCount == NULL && strlen(modelManager.modelList[c].fileName) == 0)
		{
			modelManager.modelList[c]._refCount = 1;
			return &modelManager.modelList[c];
		}
	}

	
	model_delete(&modelManager.modelList[emptyIndex]);
	modelManager.modelList[emptyIndex]._refCount = 1;
	return &modelManager.modelList[emptyIndex];

	

	//slog("Failed to make a new model!");

	//return NULL;
}

Model* model_load(const char* filename)
{
	if (!filename)
		return NULL;
	Model *model;
	model = get_by_filename(filename);
	SJson* json;
	SJson* data;
	const char* str = NULL;
	const char* str2 = NULL;
	Mesh* mesh;
	Texture* texture;

	json = sj_load(filename);
	if (!json)
	{
		slog("Failed to load model %s", filename);
		return NULL;
	}

	data = sj_object_get_value(json, "model");

	if (!data)
	{
		slog("Failed to get model info from file %s!",filename);
		sj_free(json);
		return NULL;
	}

	if (model)
	{
		model->_refCount++;
		return model;
	}


	str = sj_object_get_string(data,"obj");

	if (!str)
	{
		slog("Failed to find obj data in file %s", filename);
		sj_free(json);
		return NULL;
	}

	mesh = gf3d_mesh_load_obj(str);

	if (!mesh)
	{
		slog("Failed to load obj data from model file %s", filename);
		sj_free(json);
		return NULL;
	}

	str2 = sj_object_get_string(data, "texture");

	if (!str2)
	{
		texture = gf3d_texture_load(str2);
		if (!texture)
		{
			texture = modelManager.defaultTexture;
		}
	}
	else
	{
		texture = modelManager.defaultTexture;
	}
	

	model = modelNew();
	if (!model)
	{
		slog("Failed to get an emty space in memory for a model: %s", filename);
		return NULL;
	}

	model->mesh = mesh;
	model->texture = texture;
	gfc_line_cpy(model->fileName,filename);
	return model;
	
}

//I think this is right
Model* get_by_filename(const char *filename)
{
	int c;
	if (!filename)
		return NULL;

	for (c = 0; c < modelManager.modelCount; c++)
	{
		if (modelManager.modelList[c]._refCount == 0)
			continue;
		if (modelManager.modelList[c].fileName == NULL)
			continue;

		if (gfc_strlcmp(modelManager.modelList[c].fileName, filename) == 0)
			return &modelManager.modelList[c];
	}

	return NULL;

}


//I think this is wrong
ModelUBO model_get_ubo(GFC_Matrix4 modelMat,GFC_Color colormod)
{
	ModelUBO ubo = { 0 };
	GFC_Matrix4* view;
	gfc_matrix4_copy(ubo.model,modelMat);
	view = gf3d_vgraphics_get_view_matrix();
	if (view)
	{
		gfc_matrix4_copy(ubo.view, &view);
	}
	gf3d_vgraphics_get_projection_matrix(&ubo.proj);
	ubo.color = gfc_color_to_vector4f(colormod);
	return ubo;
}


void gf3d_model_queue_render(Model* model,GFC_Matrix4 mat,GFC_Color colormod)
{
	if (!model)
		return;
	
	ModelUBO ubo = model_get_ubo(mat, colormod);
	gf3d_mesh_queue_render(model->mesh,modelManager.pipe,&ubo ,model->texture); //IDK
	//gf3d_pipeline_queue_render(model_manager.pipe,mesh);
}

/* Not used
void gf3d_mesh_reset_pipes()
{
	Uint32 bufferFrame = gf3d_vgraphics_get_current_buffer_frame)_;
	
}
*/

//become Sky
void sky_queue_render(Model* model, GFC_Matrix4 mat,GFC_Color color)
{
	//IDK
	ModelUBO ubo;
	if (!model)
		return;

	ubo = model_get_ubo(mat, color);
	
	//I think this is wrong
	gf3d_mesh_queue_render(model->mesh,modelManager.pipe,&ubo,model->texture);
}

Pipeline* modelGetPipeline()
{
	return modelManager.pipe;
}


