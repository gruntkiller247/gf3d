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

}ModelManager;



static ModelManager model_manager = { 0 };

void model_init_system(Uint32 modelCount)
{
	//Pipeline made here?
	//Stolen from sprite?

	//Steal the mesh creation code and replace here
}