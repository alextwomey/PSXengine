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
int defX = 3500;
int defY = -1000;
int defZ = -3500;

MyPad pad;


//main program 
int main(void){   
    loadedObjects = 0;
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
    //readFromCd("YOO.VAG",&cdData[5]);
    //readFromCd("0_come.vag",&cdData[6]);
    //readFromCd("1_cuek.vag",&cdData[7]);
    //readFromCd("2_erro.vag",&cdData[8]);
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
;
    setObjectPos(&myObjects[0],0,3500,0);
    setObjectSca(&myObjects[0],1700,1700,1700);

    setObjectPos(&myObjects[1],3500,-924,-3500);
    setObjectSca(&myObjects[1],1700,1700,1700);

    setObjectPos(&myObjects[2],4500,-924,-2500);
    setObjectSca(&myObjects[2],1700,1700,1700);

    setObjectPos(&myObjects[3],2500,-524,-4500);
    setObjectSca(&myObjects[3],1700,1700,1700);

    
    loadTexture((u_char*)cdData[0]);
    start3D();
    InitializeAllLights();

    //loadedObjects += LoadTMD(cdData[1],&myObjects[loadedObjects],0,loadedObjects);//grid
    //loadedObjects++;
    loadedObjects += LoadTMD(cdData[2],&myObjects[loadedObjects],1,loadedObjects);//yoshi
    loadedObjects += LoadTMD(cdData[2],&myObjects[loadedObjects],1,loadedObjects);//yoshi
    loadedObjects += LoadTMD(cdData[2],&myObjects[loadedObjects],1,loadedObjects);//yoshi

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
    FntPrint("Count: %d, Seconds: %d, FPS: %d \n",count,seconds,fps);
    FntPrint("FrameTime: %d\n", frameTime);
    FntPrint("Yoshis: %d\n", loadedObjects);
    FntPrint("Yoshi Loc x: %d y: %d z: %d \n",myObjects[1].pos.vx,myObjects[1].pos.vy,myObjects[1].pos.vz);    
    FntPrint("Yoshi sca x: %d y: %d z: %d \n",myObjects[1].sca.vx,myObjects[1].sca.vy,myObjects[1].sca.vz);    
    FntPrint("Yoshi rot x: %d y: %d z: %d \n",myObjects[1].rot.vx,myObjects[1].rot.vy,myObjects[1].rot.vz);   
    FntPrint( "Pad 1 : %02x\nButtons:%02x %02x,\n Stick:RX:%d RY:%d LX:%d LY:%d\n",
                theControllers[0].type,             // Controller type : 00 == none,  41 == standard, 73 == analog/dualshock, 12 == mouse, 23 == steering wheel, 63 == gun, 53 == analog joystick
                theControllers[0].button1,          // 
                theControllers[0].button2,
                pad.analogRightX,
                pad.analogRightY,
                pad.analogLeftX,
                pad.analogLeftY );
    GsSortFastSprite(&myBgSprite, &orderingTable[myActiveBuff], 64);
    CalculateCamera();

    //setObjectRot(&myObjects[0],myObjects[0].rot.vx,myObjects[0].rot.vy -=10,myObjects[0].rot.vz);
    //setObjectRot(&myObjects[1],myObjects[1].rot.vx,myObjects[1].rot.vy +=10,myObjects[1].rot.vz);
    //setObjectRot(&myObjects[2],myObjects[2].rot.vx,myObjects[2].rot.vy -=10,myObjects[2].rot.vz-=10);
    //setObjectRot(&myObjects[3],myObjects[3].rot.vx,myObjects[3].rot.vy -=10,myObjects[3].rot.vz);
    
    if(pad.left){
        if(!pad.prevLeft){
            loadedObjects += LoadTMD(cdData[2],&myObjects[loadedObjects],1,loadedObjects);//yoshi
            setObjectPos(&myObjects[loadedObjects],defX+(loadedObjects*100),defY+(loadedObjects*100),defZ+(loadedObjects*100));
            setObjectSca(&myObjects[loadedObjects],1700,1700,1700);
            playSFX(&soundBank[0]);
        }
        pad.prevLeft = true;
    }
    if(!pad.left){
        pad.prevLeft = false;
    }
    if(pad.down){
        //setObjectSca(&myObjects[1],myObjects[1].sca.vx-=10,myObjects[1].sca.vy-=10,myObjects[1].sca.vz-=10);
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
        //setObjectSca(&myObjects[1],myObjects[1].sca.vx+=10,myObjects[1].sca.vy+=10,myObjects[1].sca.vz+=10);
        if(!pad.prevUp){
            playSFX(&soundBank[1]);
        }
        pad.prevUp = true;
    }
    if(!pad.up){
        pad.prevUp = false;
    }
    
    for(int i = 0; i < loadedObjects; i++){
        
        setObjectRot(&myObjects[i],myObjects[i].rot.vx,myObjects[i].rot.vy -=10,myObjects[i].rot.vz);
        RenderObject(&myObjects[i]);
    }

    display();
}

void update(){
    updateControls(&theControllers[0],&pad);
    
}