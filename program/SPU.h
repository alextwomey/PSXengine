#pragma once
#include <libspu.h>
#include <libcd.h>

extern SpuCommonAttr spuSettings;
extern SpuVoiceAttr  voiceAttributes ;          // structure for changing individual voice attributes
extern u_long vag_spu_address;                  // address allocated in memory for first sound file
// DEBUG : these allow printing values for debugging
extern u_long spu_start_address;                
extern u_long get_start_addr;
extern u_long transSize;  
typedef struct VAGheader{       // All the values in this header must be big endian
        char id[4];             // VAGp         4 bytes -> 1 char * 4
        unsigned int version;          // 4 bytes
        unsigned int reserved;         // 4 bytes
        unsigned int dataSize;         // (in bytes) 4 bytes
        unsigned int samplingFrequency;// 4 bytes
        char  reserved2[12];    // 12 bytes -> 1 char * 12
        char  name[16];         // 16 bytes -> 1 char * 16
        // Waveform data after that
}VAGhdr;

void SPUInitialization();
u_long sendVAGtoRAM(unsigned int VAG_data_size, unsigned char *VAG_data);
void setVoiceAttr(unsigned int pitch, long channel, unsigned long soundAddr );
void playSFX(void);
void loadVag(long* vagData, int channel);
