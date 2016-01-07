/*---------------------------------------------------------------------------------

	Simple bitmap graphics demo

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	int i;
	int mainBg;
	u16 *mainBgPtr;

	// set video mode to mode 5
	videoSetMode(MODE_5_2D);

	// map vram to start of background graphics memory
	vramSetBankA(VRAM_A_MAIN_BG);

	// initialize the background
	// bgInit(int layer, BgType type, BgSize size, int mapBase, int tileBase)
	mainBg = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

	// paint the screen red
	for(i = 0; i < 256*256; i++) {
		// note: BG_GFX assumes base 0, it's safer to do it this way, if using different bases:
		//mainBgPtr = bgGetGfxPtr(mainBg);
		//mainBgPtr[i] = RGB15(31, 0, 0) | BIT(15);
		BG_GFX[i] = RGB15(31, 0, 0) | BIT(15);
	}

	while(1) {
		swiWaitForVBlank();
	}

	return 0;
}
