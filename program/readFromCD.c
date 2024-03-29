//
// Adapted from nolibgs_hello_worlds cd lib
//
#include <libcd.h>
#include <malloc.h>
#include <stdio.h>
#include <STRINGS.H>
#include "readFromCD.h"
#include "constants.h"
#include <libspu.h>
#include"../third_party/pcdrv.h"

CdlFILE filePos = {0};
int CDreadOK = 0;
int CDreadResult = 0;
int tracks[] = {2, 0};  // Track to play , 1 is data, 2 is beach.wav, 3 is funk.wav. See isoconfig.xml
int count = 0;
int flip = 1;
u_long * dataBuffer;
SpuCommonAttr spuSettings;
CdlLOC loc[100];
int ntoc;
// Those array will hold the return values of the CD commands
u_char param[4], result[8];
u_char CtrlResult[8];
char * loadFile;

void initCD(){
    #ifdef _release_
    // Init CD system
    CdInit();
    printf("****CD reading initialized****\n");
    #else
    printf("****PCDRV reading initialized****\n");
    #endif
}
void initCDAudio(){
    #ifdef _release_
    printf("CD Audio Initialized!!\n");
    SpuInit();
    // Set master & CD volume to max
    spuSettings.mask = (SPU_COMMON_MVOLL | SPU_COMMON_MVOLR | SPU_COMMON_CDVOLL | SPU_COMMON_CDVOLR | SPU_COMMON_CDMIX);
    // Master volume should be in range 0x0000 - 0x3fff
    spuSettings.mvol.left  = 0x3fff;
    spuSettings.mvol.right = 0x3fff;
    // Cd volume should be in range 0x0000 - 0x7fff
    spuSettings.cd.volume.left = 0x7fff;
    spuSettings.cd.volume.right = 0x7fff;
    // Enable CD input ON
    spuSettings.cd.mix = SPU_ON;
    // Apply settings
    SpuSetCommonAttr(&spuSettings);
    // Set transfer mode 
    SpuSetTransferMode(SPU_TRANSFER_BY_DMA);
    // CD Playback setup
    // Play second audio track
    // Get CD TOC
    while ((ntoc = CdGetToc(loc)) == 0) { 		/* Read TOC */
        printf("No TOC found: please use CD-DA disc...\n");
    }
    // Prevent out of bound pos
    for (int i = 1; i < ntoc; i++) {
        CdIntToPos(CdPosToInt(&loc[i]) - 74, &loc[i]);
        printf("%s", (char*)&loc[i]);
    }
    // Set CD parameters ; Report Mode ON, CD-DA ON. See LibeOver47.pdf, p.188
    param[0] = CdlModeRept|CdlModeDA;
    CdControl(CdlSetmode, param, 0);	/* set mode */
    #else
    printf("CD audio NOT INIT, debug mode on\n");
    #endif
}

void playMusicFromCD(int trackNum){
    #ifdef _release_
    // Play second track in toc array
    CdControl(CdlPlay, (u_char *)&loc[trackNum], 0);	/* play */	
    #else
    printf("not playing audio, debug mode on\n");
    #endif
}

//Read from cd method
void readFromCd(unsigned char* filePath, long** file){
    #ifdef _release_
    // Set name of file to load
    loadFile = malloc3(3+strlen(filePath));
    strcpy(loadFile,"\\");
    strcat(loadFile,filePath);
    strcat(loadFile,";1");
    // Get file position from filename
    printf("Looking for file %s\n",loadFile);
    CdSearchFile( &filePos, loadFile);
    // Allocate memorys
    printf("file size: %lu\n",filePos.size);
    printf("file size in BtoS format: %lu\n",BtoS(filePos.size) * CD_SECTOR_SIZE );
    printf("file name: %s\n",filePos.name);
    dataBuffer = malloc3(BtoS(filePos.size) * CD_SECTOR_SIZE );
    CdControl(CdlSetloc, (u_char *)&filePos.pos, CtrlResult);
    // Read data and load it to dataBuffer
    CDreadOK = CdRead( (int)BtoS(filePos.size), (u_long *)dataBuffer, CdlModeSpeed );
    if(CDreadOK == 1){
        printf("its good \n");
    }
    else{printf("its bad\n");}
    // Wait for operation to complete
    CDreadResult = CdReadSync(0, 0);
    if(CDreadResult == 0){
        printf("read sync finished no sectors left\n");
        *file = dataBuffer;
    }else{
        printf("read sync has %d sectors left\n",CDreadResult);
    }
    free3(loadFile);
    free3(dataBuffer);
    #else
    //pcdrv
    int handler = -1;
    int lastOpsVal = 0;
    char* loadFile = malloc3(8+strlen(filePath));
    strcpy(loadFile,"assets/");
    strcat(loadFile,filePath);
    printf("Loading file from PCDRV: %s\n", loadFile);
    handler = PCopen( loadFile, FILEMODE_READONLY, 0);
	if(handler == -1){
		printf("File Not Found %s\n",loadFile);
	}
	else{
		printf("File Found!!! %s\n",loadFile);
		int fileSize = PClseek( handler, 0, 2 );
			if ( fileSize == -1 ){
				printf( "Couldn't seek to find the file size...\n" );
			} else {
				int returnToStart;
				dataBuffer = malloc3(fileSize);
				returnToStart = PClseek( handler, 0, 0 );
				if ( fileSize == -1 ){
                        printf( "Couldn't seek back to the start of the file...\n" );
                    } else {
						lastOpsVal = PCread(handler,dataBuffer, fileSize);
						if ( lastOpsVal == -1 ){
                            printf("Error reading the file!\n");
                        } else {
                            printf("Loaded file with size: %d\n",fileSize);
                            *file = dataBuffer;
                        }
					}
			}
				lastOpsVal = PCclose(handler);
				if(lastOpsVal == -1){
					printf("File Closing Error!!\n");
				}else{
					printf("File Closed!\n");
				}
	}
	free3(loadFile);
    free3(dataBuffer);
    #endif
}



        // Print heap and buffer addresses
       // FntPrint("Heap: %x - Buf: %x\n", ramAddr, dataBuffer);
        // Print returned values
       // FntPrint("CdCtrl: %d\nRead  : %d %d\n", CtrlResult[0], CDreadOK, CDreadResult);
        // Print filesize in bytes/sectors
       // FntPrint("Size: %dB sectors: %d", filePos.size, BtoS(filePos.size));
    // Get current track number ~ every second
            // See LibeOver47.pdf, p.188
           // CdReady(1, &result[0]);
            // current track number can also be obtained with 
           // CdControl(CdlGetlocP, 0, &result[0]);
