/*---------------------------------------------------------------------------------

	Simple touch demo

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	
	touchPosition touch;

	videoSetMode(MODE_FB0);
	vramSetBankA(VRAM_A_LCD);

	lcdMainOnBottom();

	while(1) {

		scanKeys();

		if(keysHeld() & KEY_TOUCH) {

			// read the current position of the touch
			touchRead(&touch);

			// draw a pixel where we touch
			VRAM_A[(touch.py * SCREEN_WIDTH) + touch.px] = rand();
		}
	}

	return 0;
}
