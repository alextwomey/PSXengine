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

    create_sprite((u_char*)cdData[4],0,0,&myBgSprite,1);
    

    myBgSprite.x = SCREENLEFTX;
    myBgSprite.y = SCREENTOPY;
    myBgSprite.w = SCREENXRES;
    myBgSprite.h = SCREENYRES;

    
    myObjects[0].pos.vx = 0;
    myObjects[0].pos.vy = 3500;
    myObjects[0].pos.vz = 0;
    myObjects[0].sca.vx = 1700;
    myObjects[0].sca.vy = 1700;
    myObjects[0].sca.vz = 1700;

    myObjects[1].pos.vx = 3500;
    myObjects[1].pos.vy = -924;
    myObjects[1].pos.vz = -3500;
    myObjects[1].sca.vx = 1700;
    myObjects[1].sca.vy = 1700;
    myObjects[1].sca.vz = 1700;

    myObjects[2].pos.vx = 4500;
    myObjects[2].pos.vy = -924;
    myObjects[2].pos.vz = -2500;
    myObjects[2].sca.vx = 1700;
    myObjects[2].sca.vy = 1700;
    myObjects[2].sca.vz = 1700;
    
    myObjects[3].pos.vx = 2500;
    myObjects[3].pos.vy = -524;
    myObjects[3].pos.vz = -4500;
    myObjects[3].sca.vx = 1700;
    myObjects[3].sca.vy = 1700;
    myObjects[3].sca.vz = 1700;
    

    loadTexture((u_char*)cdData[0]);
    start3D();
    LoadTMD(cdData[1],&myObjects[0],0);//grid
    LoadTMD(cdData[2],&myObjects[1],0);//yoshi
    LoadTMD(cdData[2],&myObjects[2],0);//yoshi
    LoadTMD(cdData[2],&myObjects[3],0);//yoshi
    
    VSyncCallback(vsync_cb);
    while (1)  // infinite loop
    {   
        count ++;
        if(count % 60 == 0){
            seconds++;
        }
        update();
        render();
        fps_measure++;
    }
    return 0;
}



void render() {

    clear_display();
    FntPrint("Count: %d, Seconds: %d \n",count,seconds);
    FntPrint("FPS: %d\n", fps);
    FntPrint("Yoshi Location x: %d y: %d z: %d \n",myObjects[1].pos.vx,myObjects[1].pos.vy,myObjects[1].pos.vz);    
    FntPrint("Yoshi Scale x: %d y: %d z: %d \n",myObjects[1].sca.vx,myObjects[1].sca.vy,myObjects[1].sca.vz);    
    FntPrint("Yoshi rotation x: %d y: %d z: %d \n",myObjects[1].rot.vx,myObjects[1].rot.vy,myObjects[1].rot.vz);    
    GsSortFastSprite(&myBgSprite, &orderingTable[myActiveBuff], 64);
    CalculateCamera();

    myObjects[0].rot.vy -=10;
    
    myObjects[1].rot.vy += 10;
    myObjects[2].rot.vy -= 10;
    myObjects[2].rot.vz -= 10;
    myObjects[3].rot.vx -= 10;

    RenderObject(&myObjects[1]);
    RenderObject(&myObjects[2]);
    RenderObject(&myObjects[3]);
    RenderObject(&myObjects[0]);
    if(pad.left){
        if(!pad.prevLeft){
            playSFX(&soundBank[0]);
        }
        pad.prevLeft = true;
    }
    if(!pad.left){
        pad.prevLeft = false;
    }
    if(pad.down){
        myObjects[1].sca.vx -= 10;
        myObjects[1].sca.vy -= 10;
        myObjects[1].sca.vz -= 10;
        if(!pad.prevDown){
            playSFX(&soundBank[2]);
        }
        pad.prevDown = true;
    }
    if(!pad.down){
        pad.prevDown = false;
    }
    if(pad.right){
        if(!pad.prevright){
            playSFX(&soundBank[3]);
        }
        pad.prevright = true;
    }
    if(!pad.right){
        pad.prevright = false;
    }
    if(pad.up){
        myObjects[1].sca.vx += 10;
        myObjects[1].sca.vy += 10;
        myObjects[1].sca.vz += 10;
        if(!pad.prevUp){
            playSFX(&soundBank[1]);
        }
        pad.prevUp = true;
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
