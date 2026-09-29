#ifndef __MODEL_H__
#define __MODEL_H__
#include "gf3d_pipeline.h"
#include "gf3d_mesh.h"
#include "simple_logger.h"

typedef struct
{
	int					_refCount;	//How many entities exist that want to draw this
	Texture*			texture;	//Texture memory pointer
	Mesh				*mesh;		//GPU handles for mesh data
	//VkDescriptorSet*	descriptorSet; //
	GFC_TextLine	fileName;

}Model;

typedef struct
{
    GFC_Matrix4     model;
    GFC_Matrix4     view;
    GFC_Matrix4     proj;
    GFC_Vector4D    color;
}MeshUBO;

Model* model_load(const char* filename);

//IDK Some void function
//void 

#endif