#ifndef __DRAW_H__
#define __DRAW_H__


#include "model.h"
#include "gfc_primitives.h"


void drawInit();

void drawClose();

void drawCube(GFC_Box cube, GFC_Color);

void drawCylinder(GFC_Box cube, GFC_Color);

void drawSphere(GFC_Sphere sphere, GFC_Color);


#endif