#include "readFromCD.h"
#include "SPU.h"

// SPU attributes
SpuCommonAttr spuSettings;

void SPUInitialization(){
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
}