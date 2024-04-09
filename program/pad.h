
#pragma once
#include <libpad.h>
#include <libetc.h>
#include <libapi.h>
#include <stdbool.h>
// hello_libpad example
//
// We're using libpad this time.
// You can use the classic controller, analog, wheel, gun buttons or mouse
//
// Schnappy - 12/2020


// Pad stuff
// Structure for storing processed controller data
typedef struct
{
    int             xpos, ypos;     // Stored position for sprite(s)
    int             xpos2, ypos2;   // controlled by this controller.
    unsigned char   status;         // These 8 values are obtained
    unsigned char   type;           // directly from the controller
    unsigned char   button1;        // buffer we installed with InitPAD.
    unsigned char   button2;
    unsigned char   analog0;
    unsigned char   analog1;
    unsigned char   analog2;
    unsigned char   analog3;
} Controller_Data;
// All-purpose controller data buffer
typedef struct
{
    unsigned char pad[34];          // 8-bytes w/o Multi-Tap, 34-bytes w/Multi-Tap
} Controller_Buffer;

typedef struct{
    bool left;
    bool right;
    bool up;
    bool down;
    bool start;
    bool select;
    bool x;
    bool t;
    bool s;
    bool c;
    bool l1;
    bool l2;
    bool r1;
    bool r2;
    bool l3;
    bool r3;
    bool prevLeft;
    bool prevright;
    bool prevUp;
    bool prevDown;
    bool prevStart;
    bool prevSelect;
    bool prevX;
    bool prevT;
    bool prevS;
    bool prevC;
    bool prevL1;
    bool prevL2;
    bool prevR1;
    bool prevR2;
    bool prevL3;
    bool prevR3;
    int analogLeftX;
    int analogLeftY;
    int analogRightX;
    int analogRightY;
    int prevAnalogLeftX;
    int prevAnalogLeftY;
    int prevAnalogRightX;
    int prevAnalogRightY;
} MyPad;

extern Controller_Buffer controllers[2];   // Buffers for reading controllers
extern Controller_Data theControllers[8];  // Processed controller data


void read_controller( Controller_Data *c, unsigned char *buf, int port );
void get_digital_direction( Controller_Data *c, int buttondata ); // get analog stick values
void initializePad(void);
void updateControls(Controller_Data *con, MyPad *pad);
void normalizeSticks(Controller_Data *con, MyPad *pad);

