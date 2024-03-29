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
#include <stdbool.h>

#include "constants.h"
#include "readFromCD.h"
#include "2D.h"
#include "pad.h"

void render();
void controls();
void update();

GsSPRITE myBgSprite;

struct{
    bool left;
    bool right;
    bool up;
    bool down;
} pad;

//main program 
int main(void){   
    initMyHeap();
    // Init display
    init();   
    //The background color of the scene
	//in RGB (values between 0-255)
	SetBGColor(0, 21, 255);
    initializePad();
    initCD();

    //readFromCd("YOSHI.TIM",&cdData[0]);
    //readFromCd("GRID.TMD",&cdData[1]);
    //readFromCd("YOSHI.TMD",&cdData[2]);
    //readFromCd("HELO.DAT",&cdData[3]);
    readFromCd("GRAD.TIM",&cdData[4]);
    initCDAudio();
    playMusicFromCD(2);

    
    create_sprite((u_char*)cdData[4],0,0,&myBgSprite,1);
    myBgSprite.x = SCREENLEFTX;
    myBgSprite.y = SCREENTOPY;
    myBgSprite.w = SCREENXRES;
    myBgSprite.h = SCREENYRES;

    while (1)  // infinite loop
    {   
        update();
        render();
    }
    return 0;
}

void render() {

    clear_display();
    count ++;
    FntPrint("Hello CDDA !\n");  // Send string to print stream
    //FntPrint("Playback status: %d\n", result[1]);  // Send string to print stream
    FntPrint("Count: %d\n", count);
    if(pad.left){
        FntPrint("PAD LEFT PRESSED!!!\n");
    }
    if(pad.down){
        FntPrint("PAD DOWN PRESSED!!!\n");
    }
    if(pad.right){
        FntPrint("PAD RIGHT PRESSED!!!\n");
    }
    if(pad.up){
        FntPrint("PAD UP PRESSED!!!\n");
    }
    GsSortFastSprite(&myBgSprite, &orderingTable[myActiveBuff], 0);
    display();
}

void controls(){
    if(padCheck(Pad1Left)){
        pad.left = true;
    }
    else{
        pad.left = false;
    }
    if(padCheck(Pad1Right)){
        pad.right = true;
    }
    else{
        pad.right = false;
    }
    if(padCheck(Pad1Up)){
        pad.up = true;
    }
    else{
        pad.up = false;
    }
    if(padCheck(Pad1Down)){
        pad.down = true;
    }
    else{
        pad.down = false;
    }
}

void update(){
    padUpdate();
    controls();
}
