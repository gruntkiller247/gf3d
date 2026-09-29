#include "model.h"
#include "gf3d_buffers.h"
#include "gf3d_swapchain.h"
#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_commands.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"

typedef struct
{
	Model* modelList;
	Uint32 modelCount;
	Pipeline *pipe;
	VkDevice device;
	Texture* defaultTexture;

}ModelManager;



static ModelManager model_manager = { 0 };

void model_init_system(Uint32 modelCount)
{
	//Pipeline made here?
	//Stolen from sprite?

	//Steal the mesh creation code and replace here

	model_manager.defaultTexture = gf3d_texture_load("images/default.png");

	if (!model_manager.defaultTexture)
	{
		slog("DEFAULT TEXTURE DOES NOT EXIST!");
		return;
	}
}

//model_free
//movel_render
//model render generic
//model close - use model delete
//gf3d_texture_free(model_manager.defaultTexture

void model_free(Model* model)
{
	if (!model)
		return;

	model->_refCount--;
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

Model* modelNew()
{
	//Simliar to mesh, no primitives
	//If no ref count && no fl=ilenam set ref count to 1 and return a pointer
	//Otherwise if ref count is 0 and there is a file name, delete that part in memory in the array
	return NULL;
}

void model_close()
{
	return;
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
			texture = model_manager.defaultTexture;
		}
	}
	else
	{
		texture = model_manager.defaultTexture;
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

//Complete to his ingame stuff
Model* get_by_filename(const char *filename)
{
	if (!filename)
		return NULL;

	for (int c = 0; c < model_manager.modelCount; c++)
	{
		if (model_manager.modelList[c]._refCount == 0)
			continue;
		if (gfc_strcmp(model_manager.modelList[c].fileName, filename) == 0)
			return &model_manager.modelList[c].fileName;
	}

	return NULL;

}



ModelUBO model_get_ubo(
	GFC_Matrix4 modelMat,
	GFC_Color colormod)
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
}


void gf3d_model_queue_render(Model* model,GFC_Matrix4 mat,GFC_Color colormod)
{
	if (!model)
		return;

	UboData ubo = model_get_ubo(mat, colorMod);
	gf3d_mesh_queue_render(model->mesh,model_manager.pipe,void* uboData,model->texture);
	//gf3d_pipeline_queue_render(model_manager.pipe,mesh);
}

/* Not used
void gf3d_mesh_reset_pipes()
{
	Uint32 bufferFrame = gf3d_vgraphics_get_current_buffer_frame)_;
	
}
*/
