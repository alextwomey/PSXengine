#include "pad.h"
#include <LIBETC.H>

int SysPad = 0;
int SysPadT = 0;
//this file will include the necessary commands for reading from the gamepad and getting input.
void initializePad() {
	PadInit(0);
}

void padReset(void) {
	SysPad = 0;
	SysPadT = 0;
}

void padUpdate(void){
	int	pad = PadRead(0);
	SysPadT = pad & (pad ^ SysPad);
	SysPad = pad;
}