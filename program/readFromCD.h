#pragma once
//
// Adapted from nolibgs_hello_worlds cd lib
//
// CD library
#include <libcd.h>
#include <malloc.h>
#include <stdio.h>
#include <strings.h>
#include"../third_party/pcdrv.h"
//


// CD specifics
#define CD_SECTOR_SIZE 2048
#define FILEMODE_READONLY 0
#define FILEMODE_WRITEONLY 1
#define FILEMODE_READWRITE 2
// Converting bytes to sectors SECTOR_SIZE is defined in words, aka int
#define BtoS(len) ( ( len + CD_SECTOR_SIZE - 1 ) / CD_SECTOR_SIZE ) 

// libcd's CD file structure contains size, location and filename
extern CdlFILE filePos;
//~ struct EXEC * exeStruct;
extern char * loadFile;
// Load data to this buffer
extern u_long * dataBuffer;              
// Those are not strictly needed, but we'll use them to see the commands results.
// They could be replaced by a 0 in the various functions they're used with.
extern u_char CtrlResult[8];
// Value returned by CDread() - 1 is good, 0 is bad
extern int CDreadOK;
// Value returned by CDsync() - Returns remaining sectors to load. 0 is good.
extern int CDreadResult;
// CD tracks 
extern int playing;
extern int tracks[];  // Track to play , 1 is data, 2 is beach.wav, 3 is funk.wav. See isoconfig.xml

//cd stuff
extern int count;
extern int flip;
extern CdlLOC loc[100];
extern int ntoc;
// Those array will hold the return values of the CD commands
extern u_char param[4], result[8];

void initCD();
void initCDAudio();
void playMusicFromCD(int trackNum);
//Read from cd method
void readFromCd(unsigned char* filePath, long** file);