#include <STDLIB.H>
#include <STDIO.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBETC.H>
#include <LIBSPU.H>
#include <LIBDS.H>
#include <LIBCD.H>
#include <sys/types.h>
#include <STRINGS.H>
#include "constants.h"


GsDOBJ2 Object[MAX_OBJECTS];
int ObjectCount;
short db = 0;  
int myActiveBuff=0;
long* cdData[20];
unsigned char ramAddr[0x00100000];
Color BGColor;
GsOT orderingTable[2];
GsOT_TAG orderingTable_TAG[2][OT_ENTRIES];
PACKET GPUOutputPacket[2][PACKETMAX*24];
Camera myCamera;
//Creates a color from RGB
Color createColor(int r, int g, int b) {
	Color color = {.r = r, .g = g, .b = b};
	return color;
}

void SetBGColor (int r, int g, int  b) {
	BGColor = createColor(r, g, b);
}

void initMyHeap(){
	// Init heap
    InitHeap3((u_long *)ramAddr, sizeof(ramAddr));
}

void init()
{
	#ifdef _release_
	printf("***RELEASE MODE***\n");
	#else
	printf("===DEBUG MODE===\n");
	#endif
	//ResetGraph(0);
	SetVideoMode(0);
	printf("Video mode is(%ld)\n",GetVideoMode());
	printf("SCREENXRES: %d, SCREENYRES: %d\n",SCREENXRES,SCREENYRES);
	//new gs init
	GsInitGraph(SCREENXRES,SCREENYRES,GsNONINTER|GsOFSGPU,1,0);
	GsDefDispBuff(0, 0, 0, SCREENYRES);
	// Prepare the ordering tables
	orderingTable[0].length=OT_LENGTH;
	orderingTable[1].length=OT_LENGTH;
	orderingTable[0].org=orderingTable_TAG[0];
	orderingTable[1].org=orderingTable_TAG[1];

	GsClearOt(0, 0, &orderingTable[0]);
	GsClearOt(0, 0, &orderingTable[1]);
	
	FntLoad(960, 0);           // Load font to vram at 960,0(+128)
    FntOpen(-120,0,SCREENXRES, SCREENYRES, 0, 512);

	// Setup 3D and projection matrix
	GsInit3D();
	GsSetProjection(CENTERX);
	GsInitCoordinate2(WORLD, &myCamera.coord2);

	// Set default lighting mode
	//0 = No Fog
	//1 = Fog
	GsSetLightMode(1);
}

void clear_display() {
	// Get active buffer ID and clear the OT to be processed for the next frame
	myActiveBuff = GsGetActiveBuff();
	GsSetWorkBase((PACKET*)GPUOutputPacket[myActiveBuff]);
	GsClearOt(0, 0, &orderingTable[myActiveBuff]);

}

void display() {

	FntFlush(-1);

	DrawSync(0);
	VSync(0);
	GsSwapDispBuff();
	//the first 3 numbers are the background color
	//was 0, 64, 0
	GsSortClear(BGColor.r, BGColor.g, BGColor.b, &orderingTable[myActiveBuff]);
	GsDrawOt(&orderingTable[myActiveBuff]);
	myActiveBuff = myActiveBuff^1;
}
