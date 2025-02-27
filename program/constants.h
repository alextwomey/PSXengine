#pragma once
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


//**********************
//Release def (switches cd read and pcdrv read)
//**********************
#define _release_
//**********************


#define VMODE 0                 // Video Mode : 0 : NTSC, 1: PAL
#define SCREENXRES 320          // Screen width
#define SCREENYRES 240         // Screen height : If VMODE is 0 = 240, if VMODE is 1 = 256
#define SCREENLEFTX -160;
#define SCREENTOPY -120; 
#define CENTERX SCREENXRES/2    // Center of screen on x 
#define CENTERY SCREENYRES/2    // Center of screen on y
#define MARGINX 32             // margins for text display
#define MARGINY 32
#define FONTSIZE 8 * 7           // Text Field Height
#define OT_LENGTH	12
#define OT_ENTRIES	1<<OT_LENGTH
#define PACKETMAX	2048
#define FRACTIONAL_BITS 16
#define FIXED_POINT_FACTOR (1 << FRACTIONAL_BITS)


extern DISPENV disp[2];                 // Double buffered DISPENV and DRAWENV
extern DRAWENV draw[2];
extern short db;                     // index of which buffer is used, values 0, 1
//Store all your CD Files Here
//the number is how many files
//you eventually want to load.
extern long* cdData[20];

extern GsOT orderingTable[2];
extern GsOT_TAG	orderingTable_TAG[2][OT_ENTRIES];
extern int myActiveBuff;
extern PACKET GPUOutputPacket[2][PACKETMAX*24];

//long for fps tracking
extern int vsyncInterval;
extern int fps;
extern int fps_counter;
extern int fps_measure;
extern int frameTime;
extern int dt;
// Define start address of allocated memory
// Let's use an array so we don't have to worry about using a memory segment that's already in use.
extern unsigned char ramAddr[0x00100000]; // https://discord.com/channels/642647820683444236/663664210525290507/864936962199781387

// Increasing this value (max is 14) reduces sorting errors in certain cases

//structs here
// Camera coordinates
typedef struct {
	VECTOR	position;
	SVECTOR rotation;
	GsCOORDINATE2 coord2;
} Camera;
extern Camera myCamera;


typedef struct {
	int r;
	int g;
	int b;
} Color;

extern Color BGColor;

void vsync_cb();

//Declare Stuff Here
void clear_vram();
void initMyHeap();
//Creates a color from RGB
Color createColor(int r, int g, int b);

void SetBGColor (int r, int g, int  b);

void init(void);

void clear_display(void);

void display(void);

int fixedPointDivide(int dividend, int divisor);

int getDt();
