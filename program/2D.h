#pragma once
#include <libgs.h>
#include <stdlib.h>

#define Sprite GsSPRITE
#define ROT_ONE 4096
// image bit depths
#define FOURBITIMAGE 4412376
#define EIGHTBITIMAGE 4412377
// sprite bit patterns
#define FOURBITCOLOUR (0 << 24)
#define EIGHTBITCOLOUR (1 << 24)
#define SIXTEENBITCOLOUR (1 << 25)

typedef struct {
	RECT rect;
	RECT crect;
	GsIMAGE tim_data;
	GsSPRITE sprite;
} Image;

void create_sprite(unsigned char imageData[],int startX,int startY,GsSPRITE* sprite,int bitMode);
void draw_sprite(Sprite* sprite);