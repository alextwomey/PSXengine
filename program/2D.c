#include <stdio.h>
#include "2d.h"
#include "constants.h"
#include <libgte.h>

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
    //printf("pmode: %lu\n",tim_data->pmode);
    switch(tim_data->pmode){
        case FOURBITIMAGE:
            printf("four bit sprite\n");
            sprite->attribute = FOURBITCOLOUR;
            sprite->w = rect->w*4;
            sprite->tpage = getTPage(0, 0, rect->x, rect->y);   
            break;
        case EIGHTBITIMAGE:
            printf("eight bit sprite\n");
            sprite->attribute = EIGHTBITCOLOUR;
            sprite->w = rect->w*2;
            sprite->tpage = getTPage(1, 0, rect->x, rect->y);
            break;
        default:
            printf("16 bit sprite\n");
            sprite->attribute = SIXTEENBITCOLOUR;
            sprite->w = rect->w;
            sprite->tpage = getTPage(2,0,rect->x,rect->y);
            break;
    }
    //Initialize Sprite
    sprite -> x = startX;
    sprite -> y = startY;
    sprite -> h = rect->h;
    sprite -> r = 128;
    sprite -> g = 128;
    sprite -> b = 128;
    sprite -> u = (rect->x*4)%256;
    sprite -> v = ((rect->y%256));
    sprite -> cx = crect->x;
    sprite -> cy = crect->y;
    sprite -> rotate = ROT_ONE * 0;            // Rotation, ROT_ONE * (0 to 360)
	sprite -> mx = rect->h/2;                          // Rotation x coord
	sprite -> my = rect->h/2;                          // Rotation y coord
	sprite -> scalex = ONE * 1.25;				// Sprite scale (width)
	sprite -> scaley = ONE * 1.25;				// Sprite scale (height)

    // Clean up
	free3(rect);
	free3(crect);
	free3(tim_data);
}
