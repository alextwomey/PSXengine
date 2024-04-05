#pragma once
#include <libgs.h>
#include <STDLIB.H>
#include "constants.h"

// Maximum number of objects
#define MAX_OBJECTS 100
typedef struct{
    char name[10];
    VECTOR pos;
    SVECTOR rot;
    VECTOR sca;
    GsDOBJ2 obj;
    int objNum;
}ModelStruct;
extern ModelStruct myObjects[MAX_OBJECTS];
extern int loadedObjects;

extern Color ambientLightColor;
extern Color sunColor;
extern VECTOR sunDirection;
extern GsF_LIGHT sunLight[1];

void InitializeAllLights();
void InitializeLight(GsF_LIGHT *flLight, int nLight, int nX, int nY, int nZ, int nRed, int nGreen, int nBlue);


ModelStruct create3DModel(u_char * model);
void start3D();
int LoadTMD(u_long *tmd, ModelStruct *mod, int enableLighting,int loadedObj);
void RenderObject(ModelStruct *mod);
void CalculateCamera();
void setSunDirection(int x, int y, int z);
void setSunColor(int r, int g, int b);
void setAmbientLight(int r, int g, int b);
void loadTexture(unsigned char imageData[]);
void setObjectPos(ModelStruct* mod,int x, int y, int z);
void setObjectRot(ModelStruct* mod,int x, int y, int z);
void setObjectSca(ModelStruct* mod,int x, int y, int z);