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
#include "SPU.h"

void render();
void controls();
void update();

GsSPRITE myBgSprite;
int seconds = 0;
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
    SPUInitialization();
    initCD();

    //readFromCd("YOSHI.TIM",&cdData[0]);
    //readFromCd("GRID.TMD",&cdData[1]);
    //readFromCd("YOSHI.TMD",&cdData[2]);
    //readFromCd("HELO.DAT",&cdData[3]);
    readFromCd("GRAD.TIM",&cdData[4]);
    readFromCd("YOO.VAG",&cdData[5]);
    initCDAudio();
    playMusicFromCD(2);
  
    create_sprite((u_char*)cdData[4],0,0,&myBgSprite,1);
    loadVag((u_char*)cdData[5], 0);
    myBgSprite.x = SCREENLEFTX;
    myBgSprite.y = SCREENTOPY;
    myBgSprite.w = SCREENXRES;
    myBgSprite.h = SCREENYRES;

    while (1)  // infinite loop
    {   
        if(count % 60 == 0){
            seconds++;
        }
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
    FntPrint("Seconds: %d\n", seconds);
    FntPrint("\nSet Start addr    : %08x", vag_spu_address);
    FntPrint("\nReturn start addr : %08x", spu_start_address);      
    FntPrint("\nGet Start  addr   : %08x", get_start_addr);  
    FntPrint("\nReturn size       : %08x\n", transSize); 
    if(pad.left){
        FntPrint("PAD LEFT PRESSED!!!\n");
        playSFX();
    }
    if(pad.down){
        FntPrint("PAD DOWN PRESSED!!!\n");
        playSFX();
    }
    if(pad.right){
        FntPrint("PAD RIGHT PRESSED!!!\n");
        playSFX();
    }
    if(pad.up){
        FntPrint("PAD UP PRESSED!!!\n");
        playSFX();
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
