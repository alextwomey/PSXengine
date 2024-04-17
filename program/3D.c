#include <stdio.h>
#include "3D.h"
#include "constants.h"
#include <libgte.h>

Color ambientLightColor;
Color sunColor;
VECTOR sunDirection;
GsF_LIGHT sunLight[1];
ModelStruct myObjects[MAX_OBJECTS];
int loadedObjects;
GsF_LIGHT flLights[2];

void setAmbientLight(int r, int g, int b){
    ambientLightColor = createColor(r,g,b);
    GsSetAmbient(ambientLightColor.r * (ONE/255),
                 ambientLightColor.g * (ONE/255),
                 ambientLightColor.b * (ONE/255) 
    );
}

void setSunColor(int r, int g, int b){
    sunColor = createColor(r,g,b);
}

void setSunDirection(int x, int y, int z){
    sunDirection.vx = x;
    sunDirection.vy = y;
    sunDirection.vz = z;
}

void CalculateCamera() {

	// This function simply calculates the viewpoint matrix based on the camera coordinates...
	// It must be called on every frame before drawing any objects.

	VECTOR	vec;
	GsVIEW2 view;

	// Copy the camera (base) matrix for the viewpoint matrix
	view.view = myCamera.coord2.coord;
	view.super = WORLD;

	//Find rotation matrix from a rotation angle.
	//input SVECTOR, output MATRIX
	RotMatrix(&myCamera.rotation, &view.view);
	//Multiply a vector by a matrix.
	//Input MATRIX, input VECTOR, output VECTOR
	ApplyMatrixLV(&view.view, &myCamera.position, &vec);
	//Set the amount of parallel transfer
	//Output MATRIX, input VECTOR
	TransMatrix(&view.view, &vec);

	// Set the viewpoint matrix to the GTE
	GsSetView2(&view);

	// Set the light source coordinates
	sunLight[0].vx = sunDirection.vx;
	sunLight[0].vy = -sunDirection.vy;
	sunLight[0].vz = sunDirection.vz;

	sunLight[0].r =	sunColor.r;	sunLight[0].g = sunColor.g;	sunLight[0].b = sunColor.b;
	GsSetFlatLight(0, &sunLight[0]);

}

void InitializeAllLights(){
	InitializeLight(&flLights[0],0,451,3771,0,255* (ONE/255),0* (ONE/255),0* (ONE/255));
	InitializeLight(&flLights[1],1,408,1512,0,0* (ONE/255),0* (ONE/255),255* (ONE/255));
	GsSetAmbient(255,255,255);
}
void InitializeLight(GsF_LIGHT *flLight, int nLight, int nX, int nY, int nZ, int nRed, int nGreen, int nBlue){
	flLight->vx=nX;flLight->vy=nY;flLight->vz=nZ;
	flLight->r=nRed;flLight->g=nGreen;flLight->b=nBlue;
	GsSetFlatLight(nLight, flLight);
}

void RenderObject(ModelStruct *mod) {

	MATRIX lmtx,omtx;
	GsCOORDINATE2 coord;

	// Copy the camera (base) matrix for the model
	coord = myCamera.coord2;

	//Flip the Y axis so a positive value
	//is up, and a negative value is down
	//mod->pos.vy *= -1;

	// Rotate and translate the matrix according to the specified coordinates
	RotMatrix(&mod->rot, &omtx);
	TransMatrix(&omtx, &mod->pos);
	ScaleMatrix(&omtx,&mod->sca);
	CompMatrixLV(&myCamera.coord2.coord, &omtx, &coord.coord);
	coord.flg = 0;

	// Apply coordinate matrix to the object
	mod->obj.coord2 = &coord;

	// Calculate Local-World (for lighting) and Local-Screen (for projection) matrices and set both to the GTE
	GsGetLws(mod->obj.coord2, &lmtx, &omtx);
	GsSetLightMatrix(&lmtx);
	GsSetLsMatrix(&omtx);


	// Sort the object!
	GsSortObject4(&mod->obj, &orderingTable[myActiveBuff], 1-OT_LENGTH, getScratchAddr(0));

}

int LoadTMD(u_long *tmd, ModelStruct *mod, int enableLighting, int loadedObj) {

	/*	This function prepares the specified TMD model for drawing and then
		links it to a GsDOBJ2 structure so it can be drawn using GsSortObject4().

		By default, light source calculation is disabled but can be re-enabled by
		simply setting the attribute variable in your GsDOBJ2 structure to 0.

		Parameters:
			*tmd - Pointer to a TMD model file loaded in memory.
			*obj - Pointer to an empty GsDOBJ2 structure.

		Returns:
			Number of objects found inside the TMD file.

	*/

	u_long *dop;
	int i,NumObj;

	// Copy pointer to TMD file so that the original pointer won't get destroyed
	dop = tmd;

	// Skip header and then remap the addresses inside the TMD file
	dop++;
	GsMapModelingData(dop);

	// Get object count
	dop++;
	NumObj = *dop;

	// Link object handler with the specified TMD
	dop++;
	mod->objNum = loadedObj;
	for(i=0; i<NumObj; i++) {
		GsLinkObject4((u_long)dop, &mod->obj, i);
		//connect the WORLD coordinate directly
		//GsInitCoordinate2(WORLD,obj[i].coord2);
		if (enableLighting == 0) {
			mod->obj.attribute = (1<<6);	// Disables light source calculation
		}
	}

	// Return the object count found inside the TMD
	return(NumObj);

}

void start3D(){
	// Setup 3D and projection matrix
	GsInit3D();
	GsSetProjection(CENTERX);
	GsInitCoordinate2(WORLD, &myCamera.coord2);

	// Set default lighting mode
	//0 = No Fog
	//1 = Fog
	GsSetLightMode(1);
	
	SetBackColor(120, 120, 120);
	SetFarColor(120, 120, 120);
	GsSetAzwh(10000, 10000, 10000);
	SetFogNearFar(300,2500,45);



    // Load TMD models

	//ObjectCount is an int defined in 3D.c
	//Every time we load a model, we increment this number.
	//Sometimes a TMD model will have more than one model
	//in the file so the LoadTMD function will return the
	//number of objects it found in whatever TMD file you loaded.

	//The LoadTMD function loads a TMD model, and stores
	//it in the Object variable defined in 3D.c

	//LoadTMD(
    //    the TMD Model to load,
    //    The Object variable to store the TMD model in,
    //    Lighting on=1 off=0
    //);

	
	//ObjectCount += LoadTMD(cdData[2], &Object[1], 0); /* Cube */

	//Set all the initial starting positions and
	//rotations here for every loaded object

	//the V in vx, vy, and vz basically stands for Vector

	//Camera
	myCamera.position.vx = 3080;
	myCamera.position.vy = -2590;
	myCamera.position.vz = -3356;

	myCamera.rotation.vx = 0;
	myCamera.rotation.vy = 2533;

	//Car
//	car.position.vx = 3500;
//	car.position.vy = 924;
//	car.position.vz = -3500;

//	car.rotation.vy = 900;

	/////////////////////////////////////////////
	////    Setup the scene lighting here    ////
	/////////////////////////////////////////////

	//The background color of the scene
	//in RGB (values between 0-255)
	SetBGColor(120, 120, 120);

	//Set the color of ambient light in RGB
	setAmbientLight(150, 150, 150);

	//The sunlight color in RGB
	setSunColor(255, 255, 255);

	//Sunlight direction
	setSunDirection(0, -1, 1);
	printf("sun set up\n");

}

void loadTexture(unsigned char imageData[]) {

	// Initialize image
	GsIMAGE* tim_data;
	RECT* rect;
	RECT* crect;
	tim_data = malloc3(sizeof(GsIMAGE));
	GsGetTimInfo ((u_long *)(imageData+4),tim_data);
	rect = malloc3(sizeof(RECT));
	crect = malloc3(sizeof(RECT));

	// Load the image into the GPU memory (frame buffer)
	rect->x = tim_data->px; // x position of image in frame buffer
	rect->y = tim_data->py; // y position of image in frame buffer
	rect->w = tim_data->pw; // width in frame buffer
	rect->h = tim_data->ph; // height in frame buffer
	printf("Framebuffer info {x=%d, y=%d, w=%d, h=%d}\n", rect->x, rect->y, rect->w, rect->h);
	LoadImage(rect, tim_data->pixel);

	// Load the color lookup table (CLUT) into the GPU memory (frame buffer)
	crect->x = tim_data->cx; // x position of CLUT in frame buffer
	crect->y = tim_data->cy; // y position of CLUT in frame buffer
	crect->w = tim_data->cw; // width of CLUT in frame buffer
	crect->h = tim_data->ch; // height of CLUT in frame buffer
	printf("CLUT info {x=%d, y=%d, w=%d, h=%d}\n", crect->x, crect->y, crect->w, crect->h);
	LoadImage(crect, tim_data->clut);

	// Clean up
	free3(rect);
	free3(crect);
	free3(tim_data);
}

void setObjectPos(ModelStruct* mod,int x, int y, int z){
	mod->pos.vx = x;
	mod->pos.vy = y;
	mod->pos.vz = z;
}
void setObjectRot(ModelStruct* mod,int x, int y, int z){
	mod->rot.vx = x;
	mod->rot.vy = y;
	mod->rot.vz = z;
}
void setObjectSca(ModelStruct* mod,int x, int y, int z){
	mod->sca.vx = x;
	mod->sca.vy = y;
	mod->sca.vz = z;
}
