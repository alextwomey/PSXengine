#include <stdlib.h>
#include <stdio.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>
#include <libspu.h>
#include <libds.h>
#include <libcd.h>
#include <sys/types.h>
#include <strings.h>
#include "constants.h"

short db = 0;  
int myActiveBuff=0;
long* cdData[20];
unsigned char ramAddr[0x00100000];
Color BGColor;
GsOT orderingTable[2];
GsOT_TAG orderingTable_TAG[2][OT_ENTRIES];
PACKET GPUOutputPacket[2][PACKETMAX*24];
Camera myCamera;
int vsyncInterval;
int fps = 60;
int fps_counter;
int fps_measure;
int frameTime;
int dt;

void vsync_cb(){
	fps_counter++;
	if(fps_counter >= 60){
		fps = fps_measure;
		fps_measure = 0;
		fps_counter = 0;
	}
}

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
	vsyncInterval = 0;
	GsInitVcount();
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
    FntOpen(-150,-100,SCREENXRES, SCREENYRES, 0, 512);

	
}

void clear_display() {
	// Get active buffer ID and clear the OT to be processed for the next frame
	myActiveBuff = GsGetActiveBuff();
	GsSetWorkBase((PACKET*)GPUOutputPacket[myActiveBuff]);
	GsClearOt(0, 0, &orderingTable[myActiveBuff]);

}

int fixedPointDivide(int dividend, int divisor){
	if(divisor == 0){
		return 0;
	}
	return (dividend * FIXED_POINT_FACTOR) / divisor;
}

int getDt(){
	return dt = fixedPointDivide(fps, 60);
}

void display() {
	//printf("display start\n");
	FntFlush(-1);
	//deltaTime = VSync(1);
	DrawSync(0);
	//VSync(0);
	
	frameTime = VSync(0);
	
	GsSwapDispBuff();
	//the first 3 numbers are the background color
	//was 0, 64, 0
	GsSortClear(BGColor.r, BGColor.g, BGColor.b, &orderingTable[myActiveBuff]);
	GsDrawOt(&orderingTable[myActiveBuff]);
	myActiveBuff = myActiveBuff^1;
	//printf("display done\n");
}
