
#include <sys/types.h>
#include <stdio.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libapi.h>
#include "readFromCD.c"

#define VMODE 0                 // Video Mode : 0 : NTSC, 1: PAL
#define SCREENXRES 320          // Screen width
#define SCREENYRES 240 + (VMODE << 4)          // Screen height : If VMODE is 0 = 240, if VMODE is 1 = 256 
#define CENTERX SCREENXRES/2    // Center of screen on x 
#define CENTERY SCREENYRES/2    // Center of screen on y
#define MARGINX 0                // margins for text display
#define MARGINY 32
#define FONTSIZE 8 * 7           // Text Field Height
DISPENV disp[2];                 // Double buffered DISPENV and DRAWENV
DRAWENV draw[2];
short db = 0;                      // index of which buffer is used, values 0, 1

//Store all your CD Files Here
//the number is how many files
//you eventually want to load.
long* cdData[3];

// Define start address of allocated memory
// Let's use an array so we don't have to worry about using a memory segment that's already in use.
static unsigned char ramAddr[0x00100000]; // https://discord.com/channels/642647820683444236/663664210525290507/864936962199781387

void init(void)
{
    ResetGraph(0);                 // Initialize drawing engine with a complete reset (0)
    SetDefDispEnv(&disp[0], 0, 0         , SCREENXRES, SCREENYRES);     // Set display area for both &disp[0] and &disp[1]
    SetDefDispEnv(&disp[1], 0, SCREENYRES, SCREENXRES, SCREENYRES);     // &disp[0] is on top  of &disp[1]
    SetDefDrawEnv(&draw[0], 0, SCREENYRES, SCREENXRES, SCREENYRES);     // Set draw for both &draw[0] and &draw[1]
    SetDefDrawEnv(&draw[1], 0, 0         , SCREENXRES, SCREENYRES);     // &draw[0] is below &draw[1]
    // Set video mode
    if (VMODE){ SetVideoMode(MODE_PAL);}
    SetDispMask(1);                 // Display on screen    
    setRGB0(&draw[0], 255, 50, 50); // set color for first draw area
    setRGB0(&draw[1], 255, 50, 50); // set color for second draw area
    draw[0].isbg = 1;               // set mask for draw areas. 1 means repainting the area with the RGB color each frame 
    draw[1].isbg = 1;
    PutDispEnv(&disp[db]);          // set the disp and draw environnments
    PutDrawEnv(&draw[db]);
    FntLoad(960, 0);                // Load font to vram at 960,0(+128)
    FntOpen(MARGINX, SCREENYRES - MARGINY - FONTSIZE, SCREENXRES - MARGINX * 2, FONTSIZE, 0, 280 ); // FntOpen(x, y, width, height,  black_bg, max. nbr. chars
}
void display(void)
{
    DrawSync(0);                    // Wait for all drawing to terminate
    VSync(0);                       // Wait for the next vertical blank
    PutDispEnv(&disp[db]);          // set alternate disp and draw environnments
    PutDrawEnv(&draw[db]);  
    db = !db;                       // flip db value (0 or 1)
}
int main(void)
{   
    // Init heap
    InitHeap((u_long *)ramAddr, sizeof(ramAddr));

    // Init display
    init();          
    CdInit();

    readFromCd("YOSHI.TIM",&cdData[0]);
    readFromCd("GRID.TMD",&cdData[1]);
    readFromCd("YOSHI.TMD",&cdData[2]);

    for(int i = 0; i > sizeof(cdData);i++){
            printf("cd data[%d]: %li",i,*cdData[i]);
        }
    

    while (1)  // infinite loop
    {   
        
        // Print the content of the loaded file - See HELO.DAT
       // FntPrint("%s%d\n", (char *)dataBuffer, VSync(-1));
        // Print heap and buffer addresses
       // FntPrint("Heap: %x - Buf: %x\n", ramAddr, dataBuffer);
        // Print returned values
       // FntPrint("CdCtrl: %d\nRead  : %d %d\n", CtrlResult[0], CDreadOK, CDreadResult);
        // Print filesize in bytes/sectors
       // FntPrint("Size: %dB sectors: %d", filePos.size, BtoS(filePos.size));
        
        //FntFlush(-1);               // Draw print stream
        display();                  // Execute display()
    }
    return 0;
    }
