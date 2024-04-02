#pragma once
#include <libgs.h>
#include <STDLIB.H>
#include "constants.h"

extern Color ambientLightColor;
extern Color sunColor;
extern VECTOR sunDirection;
extern GsF_LIGHT sunLight[1];

typedef struct{
    GsDOBJ2 gsObjectHandler;
    GsCOORDINATE2 gsObjectCoord;
}ModelStruct;

ModelStruct create3DModel(u_char * model);
void start3D();
int LoadTMD(u_long *tmd, GsDOBJ2 *obj, int enableLighting);
void RenderObject(VECTOR pos, SVECTOR rot, GsDOBJ2 *obj);
void CalculateCamera();
void setSunDirection(int x, int y, int z);
void setSunColor(int r, int g, int b);
void setAmbientLight(int r, int g, int b);
void loadTexture(unsigned char imageData[]);