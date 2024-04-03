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
#include "3D.h"

void render();
void controls();
void update();
void start();

GsSPRITE myBgSprite;
struct {
    VECTOR position;
    SVECTOR rotation;
} myTestModel;
struct {
    VECTOR position;
    SVECTOR rotation;
} myYoshiModel;

int seconds = 0;
struct{
    bool left;
    bool right;
    bool up;
    bool down;
    bool prevLeft;
    bool prevright;
    bool prevUp;
    bool prevDown;
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

    readFromCd("YOSHI.TIM",&cdData[0]);
    readFromCd("GRID.TMD",&cdData[1]);
    readFromCd("YOSHI.TMD",&cdData[2]);
    //readFromCd("HELO.DAT",&cdData[3]);
    readFromCd("GRAD.TIM",&cdData[4]);
    readFromCd("YOO.VAG",&cdData[5]);
    readFromCd("0_come.vag",&cdData[6]);
    readFromCd("1_cuek.vag",&cdData[7]);
    readFromCd("2_erro.vag",&cdData[8]);
    initCDAudio();
    playMusicFromCD(2);
    
    soundBank[0]= createVAGsound((u_char*)cdData[5],SPU_00CH,0);
    soundBank[1]= createVAGsound((u_char*)cdData[6],SPU_01CH,0);
    soundBank[2]= createVAGsound((u_char*)cdData[7],SPU_02CH,0);
    soundBank[3]= createVAGsound((u_char*)cdData[8],SPU_03CH,0);
    for(int i = 0; i < 4; i++){
        soundBank[i].spu_address = loadVag(&soundBank[i]);
        printf("Address: %lu, channel: %lu\n",soundBank[i].spu_address,soundBank[i].spu_channel);
    }

    //create_sprite((u_char*)cdData[4],0,0,&myBgSprite,1);
    

    myBgSprite.x = SCREENLEFTX;
    myBgSprite.y = SCREENTOPY;
    myBgSprite.w = SCREENXRES;
    myBgSprite.h = SCREENYRES;

    myTestModel.position.vx = 3500;
    myTestModel.position.vy = 924;
    myTestModel.position.vz = -3500;


    loadTexture((u_char*)cdData[0]);
    start3D();
    ObjectCount += LoadTMD(cdData[1],&myObjects[0],0);
    ObjectCount += LoadTMD(cdData[2],&myObjects[1],0);
    

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

    //GsSortFastSprite(&myBgSprite, &orderingTable[myActiveBuff], 64);
    CalculateCamera();
   // RenderObject(myTestModel.position, myTestModel.rotation, &myObjects[0]);
    RenderObject(myTestModel.position, myTestModel.rotation, &myObjects[1]);

    if(pad.left){
        if(!pad.prevLeft){
            playSFX(&soundBank[0]);
        }
        pad.prevLeft = true;
        FntPrint("PAD LEFT PRESSED!!!\n");
    }
    if(!pad.left){
        pad.prevLeft = false;
    }
    if(pad.down){
        if(!pad.prevDown){
            playSFX(&soundBank[2]);
        }
        pad.prevDown = true;
        FntPrint("PAD DOWN PRESSED!!!\n");
    }
    if(!pad.down){
        pad.prevDown = false;
    }
    if(pad.right){
        if(!pad.prevright){
            playSFX(&soundBank[3]);
        }
        pad.prevright = true;
        FntPrint("PAD RIGHT PRESSED!!!\n");
    }
    if(!pad.right){
        pad.prevright = false;
    }
    if(pad.up){
        if(!pad.prevUp){
            playSFX(&soundBank[1]);
        }
        pad.prevUp = true;
        FntPrint("PAD UP PRESSED!!!\n");
    }
    if(!pad.up){
        pad.prevUp = false;
    }
    
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
