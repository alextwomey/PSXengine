#include "constants.c"
#include "readFromCD.c"
#include "2D.c"

void render();

//main program 
int main(void){   
    // Init heap
    InitHeap3((u_long *)ramAddr, sizeof(ramAddr));

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
