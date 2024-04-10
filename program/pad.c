// hello_libpad example
//
// We're using libpad this time.
// You can use the classic controller, analog, wheel, gun buttons or mouse
//
// Schnappy - 12/2020
//
// Based on :  ../psyq/addons/scea/CNTRL/PAD.C

#include <libetc.h>
#include <stdio.h>
#include <stdlib.h>

#include "pad.h"

Controller_Buffer controllers[2];
Controller_Data theControllers[8];

void initializePad(){
	InitPAD(controllers[0].pad, 34, controllers[1].pad, 34);
    StartPAD();
}


void read_controller( Controller_Data *c, unsigned char *buf, int port )  // get the raw values from controller
{
    register int mouse_x, mouse_y, x;
    c->status =  buf[0];    // Copy over raw controller data
    c->type =    buf[1];
    c->button1 = buf[2];
    c->button2 = buf[3];
    c->analog0 = buf[4];
    c->analog1 = buf[5];
    c->analog2 = buf[6];
    c->analog3 = buf[7];
    if( buf[0] == 0xff )    // If controller returns BAD status then bail on it.
    {
        c->type = 0;
        return;
    }
    
}

void updateControls(Controller_Data *con, MyPad *pad){
    
    read_controller( &con[0], &controllers[0].pad[0], 0 );  // Read controllers
    normalizeSticks(&con[0], pad);
    //printf("type: %d\n",con[0].type);
    if(con[0].type != 115){
        pad->analogLeftX = 0;
        pad->analogRightX = 0;
        pad->analogLeftY = 0;
        pad->analogRightY = 0;
    }
    switch(con[0].button1){
        case 0xDF:                      // Right 
            pad->right = 1;
            break;
        case 0x7F:                     // Left  
            pad->left = 1;
            break;
        case 0xEF:                      // Up    
            pad->up = 1;
            break;
        case 0xBF:                      // Down  
            pad->down = 1;
            break;
        // Start & Select
        case 0xF7:
            pad->start = 1;
            break;
        case 0xFE:                     // SELECT
            pad->select = 1;
            break;
        // Dualshock L3 + R3
        case 0xFD:                      // L3
            pad->l3 = 1;
            break;
        case 0xFB:                      //R3
            pad->r3 = 1;
            break;
        case 0xFF:
            pad->right = 0;
            pad->left = 0;
            pad->up = 0;
            pad->down = 0;
            pad->start = 0;
            pad->select = 0;
            pad->l3 = 0;
            pad->r3 = 0;
            break;
        }
        // Buttons
        switch(theControllers[0].button2){
            case 0xDF:                      // ⭘
                pad->c = 1;
                break;
            case 0x7F:                      // ⬜
               pad->s = 1;
               break;
            case 0xEF:                      // △
                pad->t = 1;
                break;
            case 0xBF:                      // ╳
                pad->x = 1; 
                break;
                
        // Shoulder buttons             
            case 0xFB:                       // L1
                pad->l1 = 1;
                break;
            case 0xFE:                       // L2
                pad->l2 = 1;
                break;
            case 0xF7:                       // R1
                pad->r1 = 1;
                break;
            case 0xFD:                       // R2
                pad->r2 = 1;
                break;
            case 0xFF:
                pad->l1 = 0;
                pad->l2 = 0;
                pad->r1 = 0;
                pad->r2 = 0;
                pad->c = 0;
                pad->s = 0;
                pad->t = 0;
                pad->x = 0;
                break;
        // Mouse buttons 
            case 0xF4:                      // Mouse Left click
                
            case 0xF8:                      // Mouse Right click

    }
}

void normalizeSticks(Controller_Data *con, MyPad *pad){
    int rx;int ry;int lx;int ly;
    rx = (con[0].analog0 * 256)/255;
    ry = (con[0].analog1 * 256)/255;
    lx = (con[0].analog2 * 256)/255;
    ly = (con[0].analog3 * 256)/255;
    rx = (rx * 256) - (128 << 8);
    ry = (ry * 256) - (128 << 8);
    lx = (lx * 256) - (128 << 8);
    ly = (ly * 256) - (128 << 8);
    rx = rx/255;
    ry = ry/255;
    lx = lx/255;
    ly = ly/255;
    if(abs(rx)<=25){
        rx=0;
    }else{
        //linear interpolation between 20-128
    }
    if(abs(ry)<=25){
        ry=0;
    }
    if(abs(lx)<=25){
        lx=0;
    }
    if(abs(ly)<=25){
        ly=0;
    }
    pad->analogLeftX = lx;
    pad->analogLeftY = ly;
    pad->analogRightX = rx;
    pad->analogRightY = ry;
}



