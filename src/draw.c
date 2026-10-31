#include "simple_logger.h"
#include "draw.h"



typedef struct 
{
	Model* cube;
	Model* sphere;
	Model* cylinder;
}drawData;

//???????
//Trying to make a visualization draw for the bounding boxes
//static drawData data = { 0 };

void drawClose();

void drawInit()
{
	//drawData.cube = model_load("models/primitives/cube.model);
	//Make cube.obj! ETC!
	//Cylinder ignore for now
	atexit(drawClose);
}

void drawClose()
{
	//model_free(drawData.cube);
	//model_free(drawData.cylinder);
	//model_free(drawData.sphere);
}

void drawCube(GFC_Box cube, GFC_Color color)
{
	GFC_Matrix4 mat;

	gfc_matrix4_from_vectors(mat, gfc_vector3d(cube.x+(cube.w * 0.5), cube.y + (cube.h * 0.5), cube.z + (cube.d * 0.5)), gfc_vector3d(0,0,0),gfc_vector3d(cube.w,cube.h,cube.d));

	
	//gf3d_model_queue_render(drawData.cube, mat ,color);
}

void drawCylinder(GFC_Box cube)
{

}

void drawSphere(GFC_Sphere sphere)
{

}