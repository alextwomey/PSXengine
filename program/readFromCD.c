//
// Adapted from nolibgs_hello_worlds cd lib
//
// CD library
#include <libcd.h>
#include <malloc.h>
#include <stdio.h>
#include <STRINGS.H>
#include"../third_party/pcdrv.h"
//
//**********************
//Release def (switches cd read and pcdrv read)
//**********************
//#define _release_
//**********************

// CD specifics
#define CD_SECTOR_SIZE 2048
#define FILEMODE_READONLY 0
#define FILEMODE_WRITEONLY 1
#define FILEMODE_READWRITE 2
// Converting bytes to sectors SECTOR_SIZE is defined in words, aka int
#define BtoS(len) ( ( len + CD_SECTOR_SIZE - 1 ) / CD_SECTOR_SIZE ) 
// libcd's CD file structure contains size, location and filename
CdlFILE filePos = {0};
//~ struct EXEC * exeStruct;
static char * loadFile;
// Load data to this buffer
u_long * dataBuffer;              
// Those are not strictly needed, but we'll use them to see the commands results.
// They could be replaced by a 0 in the various functions they're used with.
u_char CtrlResult[8];
// Value returned by CDread() - 1 is good, 0 is bad
int CDreadOK = 0;
// Value returned by CDsync() - Returns remaining sectors to load. 0 is good.
int CDreadResult = 0;

void initCD(){
    #ifdef _release_
    // Init CD system
    CdInit();
    printf("****CD reading initialized****\n");
    #else
    printf("****PCDRV reading initialized****\n");
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
    printf("***********DEBUG VERSION LOADING FROM PCDRV**********");
    int handler = -1;
    int lastOpsVal = 0;
    loadFile = malloc3(8+strlen(filePath));
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
                            printf("Loaded File!!\n");
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