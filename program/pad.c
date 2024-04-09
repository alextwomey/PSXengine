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

#include "pad.h"

Controller_Buffer controllers[2];
Controller_Data theControllers[8];

void initializePad(){
	InitPAD(controllers[0].pad, 34, controllers[1].pad, 34);
    StartPAD();
}

void get_digital_direction( Controller_Data *c, int buttondata ) // get analog stick values
{
int i;
    i = ~(buttondata);
    if( i & 0x80 )
        c->xpos -= 1;
    if( i & 0x20 )
        c->xpos += 1;
    if( i & 0x40 )
        c->ypos += 1;
    if( i & 0x10 )
        c->ypos -= 1;
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
    // Look at the controller type code & process controller data as indicated
    switch( c->type )
    {
        case 0x12:      // Sony Mouse
            mouse_x = buf[4];
            mouse_y = buf[5];
            if( mouse_x & 0x80 )
                mouse_x |= 0xffffff80;
            if( mouse_y & 0x80 )
                mouse_y |= 0xffffff80;
            c->xpos += mouse_x;
            c->ypos += mouse_y;
            break;
        case 0x23:      // Namco negCon
                        // Steering wheel
                        // Sankyo Pachinko controler
            get_digital_direction( c, buf[2] );
            break;
        case 0x53:      // Analog 2-stick
            get_digital_direction( c, buf[2] );
            break;
        case 0x41:      // Standard Sony PAD controller
            get_digital_direction( c, buf[2] );
            break;
        default:        // If don't know what it is, treat it like standard controller
            get_digital_direction( c, buf[2] );
            break;
    }
}

void updateControls(Controller_Data *con, MyPad *pad){
    read_controller( &con[0], &controllers[0].pad[0], 0 );  // Read controllers
    normalizeSticks(&con[0], pad);
    switch(con[0].button1){
        case 0xDF:                      // Right 
            pad->right = 1;
        case 0x7F:                     // Left  
            pad->left = 1;
        case 0xEF:                      // Up    
            pad->up = 1;
        case 0xBF:                      // Down  
            pad->down = 1;
        // Start & Select
        case 0xF7:
            pad->start = 1;
        case 0xFE:                     // SELECT
            pad->select = 1;
        // Dualshock L3 + R3
        case 0xFD:                      // L3
            pad->l3 = 1;
        case 0xFB:                      //R3
            pad->r3 = 1;
        
        }
        // Buttons
        switch(theControllers[0].button2){
            case 0xDF:                      // ⭘
                pad->c = 1;
            case 0x7F:                      // ⬜
               pad->s = 1;
            case 0xEF:                      // △
                pad->t = 1;
            case 0xBF:                      // ╳
                pad->x = 1; 
                
        // Shoulder buttons             
            case 0xFB:                       // L1
                pad->l1 = 1;
            case 0xFE:                       // L2
                pad->l2 = 1;
            case 0xF7:                       // R1
                pad->r1 = 1;
            case 0xFD:                       // R2
                pad->r2 = 1;
            case 0xFF:
                pad->l1 = 0;
                pad->l2 = 0;
                pad->r1 = 0;
                pad->r2 = 0;
                pad->c = 0;
                pad->s = 0;
                pad->t = 0;
                pad->x = 0;
        // Mouse buttons 
            case 0xF4:                      // Mouse Left click
                
            case 0xF8:                      // Mouse Right click

    }
}

void normalizeSticks(Controller_Data *con, MyPad *pad){
    int rx;int ry;int lx;int ly;
    rx = (con[0].analog2 * 256)/255;
    ry = (con[0].analog3 * 256)/255;
    lx = (con[0].analog0 * 256)/255;
    ly = (con[0].analog1 * 256)/255;
    rx = (rx * 256) - (128 << 8);
    ry = (ry * 256) - (128 << 8);
    lx = (lx * 256) - (128 << 8);
    ly = (ly * 256) - (128 << 8);
    pad->analogRightX = rx/255;
    pad->analogRightY = ry/-255;
    pad->analogLeftX = lx/255;
    pad->analogLeftY = ly/-255;
}



