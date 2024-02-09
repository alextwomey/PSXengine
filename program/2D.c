#include <stdio.h>
#include "2d.h"
#include "constants.h"

void create_sprite(unsigned char imageData[],int startX,int startY,GsSPRITE* sprite,int bitMode){
    //Load texture data
    GsIMAGE* tim_data;
    RECT* rect;
    RECT* crect;
    tim_data = malloc3(sizeof(GsIMAGE));
    GsGetTimInfo((u_long*)(imageData+4),tim_data);
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

    //Initialize Sprite
    sprite = malloc3(sizeof(GsSPRITE));
    switch(tim_data->pmode){
        case FOURBITIMAGE:
        printf("four bit sprite\n");
        sprite->attribute = FOURBITCOLOUR;
        sprite->w = tim_data->pw*4;
        sprite->tpage = getTPage(0, 0, rect->x, rect->y);   

        default:
        printf("eight bit sprite\n");
        sprite->attribute = EIGHTBITCOLOUR;
        sprite->w = tim_data->pw*2;
        sprite->tpage = getTPage(1, 0, rect->x, rect->y);

        
    }
    //Initialize Sprite
    sprite -> x = startX;
    sprite -> y = startY;
    sprite -> h = tim_data->ph;
    sprite -> r = 128;
    sprite -> g = 128;
    sprite -> b = 128;
    sprite -> u = (tim_data->px*4)%256;
    sprite -> v = ((tim_data->py%256));
    sprite -> cx = tim_data->cx;
    sprite -> cy = tim_data->cy;
    sprite -> rotate = ROT_ONE * 0;            // Rotation, ROT_ONE * (0 to 360)
	sprite -> mx = tim_data->ph/2;                          // Rotation x coord
	sprite -> my = tim_data->ph/2;                          // Rotation y coord
	sprite -> scalex = ONE * 1;				// Sprite scale (width)
	sprite -> scaley = ONE * 1;				// Sprite scale (height)

    printf("Sprite data: %s\n",sprite)
    // Clean up
	free3(rect);
	free3(crect);
	free3(tim_data);
}

void draw_sprite(Sprite* sprite){
    myActiveBuff = GsGetActiveBuff();
    GsSortSprite(sprite, &orderingTable[myActiveBuff], 0);
}