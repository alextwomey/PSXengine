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

#include "constants.h"
#include "readFromCD.h"
#include "2D.h"

void render();

//main program 
int main(void){   
    initMyHeap();
    // Init display
    init();   
    //The background color of the scene
	//in RGB (values between 0-255)
	SetBGColor(0, 21, 255);
       
    initCD();

    //readFromCd("YOSHI.TIM",&cdData[0]);
    //readFromCd("GRID.TMD",&cdData[1]);
    //readFromCd("YOSHI.TMD",&cdData[2]);
    //readFromCd("HELO.DAT",&cdData[3]);
    readFromCd("GRAD.TIM",&cdData[4]);
    initCDAudio();
    playMusicFromCD(2);

    GsSPRITE* myBgSprite;
    create_sprite((u_char*)cdData[4],0,0,&myBgSprite,1);
    draw_sprite(myBgSprite);
    while (1)  // infinite loop
    {   
        render();
    }
    return 0;
}

void render() {

    clear_display();
    count ++;
    FntPrint("Hello CDDA !\n");  // Send string to print stream
    //FntPrint("Playback status: %d\n", result[1]);  // Send string to print stream
    FntPrint("Count: %d", count);
    display();
}
