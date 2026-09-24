#ifndef __MODEL_H__
#define __MODEL_H__
#include "gf3d_pipeline.h"
#include "gf3d_mesh.h"

typedef struct
{
	int					_refCount;	//How many entities exist that want to draw this
	Texture*			texture;	//Texture memory pointer
	Mesh				*mesh;		//GPU handles for mesh data
	VkDescriptorSet*	descriptorSet; //


}Model;

#endif