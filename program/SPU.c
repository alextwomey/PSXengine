#include "SPU.h"
#include "constants.h"


// SPU attributes
SpuCommonAttr spuSettings;
SpuVoiceAttr  voiceAttributes ;          // structure for changing individual voice attributes
u_long vag_spu_address;                  // address allocated in memory for first sound file
// DEBUG : these allow printing values for debugging
u_long spu_start_address;                
u_long get_start_addr;
u_long transSize;  

char spu_malloc_rec[SPU_MALLOC_RECSIZ * (MALLOC_MAX+1)];

void SPUInitialization(){
    SpuInit();
    SpuInitMalloc(MALLOC_MAX, spu_malloc_rec);
    // Set master & CD volume to max
    spuSettings.mask = (SPU_COMMON_MVOLL | SPU_COMMON_MVOLR | SPU_COMMON_CDVOLL | SPU_COMMON_CDVOLR | SPU_COMMON_CDMIX);
    // Master volume should be in range 0x0000 - 0x3fff
    spuSettings.mvol.left  = 0x3fff;
    spuSettings.mvol.right = 0x3fff;
    // Cd volume should be in range 0x0000 - 0x7fff
    spuSettings.cd.volume.left = 0x7fff;
    spuSettings.cd.volume.right = 0x7fff;
    // Enable CD input ON
    spuSettings.cd.mix = SPU_OFF;
    // Apply settings
    SpuSetCommonAttr(&spuSettings);
    // Set transfer mode 
    SpuSetTransferMode(SPU_TRANSFER_BY_DMA);
    SpuSetIRQ(SPU_OFF);
}

void setVoiceAttr(unsigned int pitch, long channel, unsigned long soundAddr ){
    voiceAttributes.mask=                                   //~ Attributes (bit string, 1 bit per attribute)
    (
      SPU_VOICE_VOLL |
      SPU_VOICE_VOLR |
      SPU_VOICE_PITCH |
      SPU_VOICE_WDSA |
      SPU_VOICE_ADSR_AMODE |
      SPU_VOICE_ADSR_SMODE |
      SPU_VOICE_ADSR_RMODE |
      SPU_VOICE_ADSR_AR |
      SPU_VOICE_ADSR_DR |
      SPU_VOICE_ADSR_SR |
      SPU_VOICE_ADSR_RR |
      SPU_VOICE_ADSR_SL
    );
    voiceAttributes.voice        = channel;                 //~ Voice (low 24 bits are a bit string, 1 bit per voice )
    voiceAttributes.volume.left  = 0x1000;                  //~ Volume 
    voiceAttributes.volume.right = 0x1000;                  //~ Volume
    voiceAttributes.pitch        = pitch;                   //~ Interval (set pitch)
    voiceAttributes.addr         = soundAddr;               //~ Waveform data start address
    voiceAttributes.a_mode       = SPU_VOICE_LINEARIncN;    //~ Attack rate mode  = Linear Increase - see libref47.pdf p.1091
    voiceAttributes.s_mode       = SPU_VOICE_LINEARIncN;    //~ Sustain rate mode = Linear Increase
    voiceAttributes.r_mode       = SPU_VOICE_LINEARDecN;    //~ Release rate mode = Linear Decrease
    voiceAttributes.ar           = 0x0;                     //~ Attack rate
    voiceAttributes.dr           = 0x0;                     //~ Decay rate
    voiceAttributes.rr           = 0x0;                     //~ Release rate
    voiceAttributes.sr           = 0x0;                     //~ Sustain rate
    voiceAttributes.sl           = 0xf;                     //~ Sustain level
    SpuSetVoiceAttr(&voiceAttributes);                      // set attributes
}

void playSFX(void){
    SpuSetKey(SpuOn,SPU_0CH);                               // Set several channels by ORing  each channel bit ; ex : SpuSetKey(SpuOn,SPU_0CH | SPU_3CH | SPU_8CH); channels 0, 3, 8 are on.
}

u_long sendVAGtoRAM(unsigned int VAG_data_size, u_char *VAG_data){
    u_long size;
    size = SpuWrite(VAG_data + sizeof(VAGhdr), VAG_data_size);
    SpuIsTransferCompleted(SPU_TRANSFER_WAIT);
    return size;
}

void loadVag(u_char *vagData,int channel){
    const VAGhdr * VAGfileHeader = (VAGhdr*)vagData;
    unsigned int pitch = (__builtin_bswap32(VAGfileHeader->samplingFrequency)<<12) / 44100L; 
    printf("PITCH:%08x\n",pitch);
    printf("data Size: %i\n",__builtin_bswap32(VAGfileHeader->dataSize));
    vag_spu_address = SpuMalloc(__builtin_bswap32(VAGfileHeader->dataSize));
    printf("Vag_SPU_ADDRESS: %08x\n",vag_spu_address);
    spu_start_address = SpuSetTransferStartAddr(vag_spu_address);
    printf("SPU Start Address:%08x\n",spu_start_address);
    get_start_addr = SpuGetTransferStartAddr();
    printf("getStart Addr: %08x\n",get_start_addr);
    transSize = sendVAGtoRAM(__builtin_bswap32((VAGfileHeader->dataSize)), vagData);
    printf("TransferSize: %08x\n",transSize);
    setVoiceAttr(pitch, SPU_0CH,vag_spu_address);
    
}