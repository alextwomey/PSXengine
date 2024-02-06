#pragma once
#include <libgs.h>
#include <STDLIB.H>

#define Sprite GsSPRITE
#define ROT_ONE 4096

typedef struct {
	RECT rect;
	RECT crect;
	GsIMAGE tim_data;
	GsSPRITE sprite;
} Image;

void create_sprite(unsigned char imageData[],int startX,int startY,GsSPRITE **sprite,int bitMode);
void draw_sprite(Sprite *sprite);