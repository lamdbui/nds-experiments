/*---------------------------------------------------------------------------------

	Simple bitmap on the main and sub screens

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	int i;

	// point our video buffer to start of bitmap background video
	u16 *videoBufferMain = (u16*)BG_BMP_RAM(0);
	u16 *videoBufferSub = (u16*)BG_BMP_RAM_SUB(0);
	// set video mode 5 with background enabled
	videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE);
	videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE);
	// map vram to start of main background graphics memory
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	vramSetBankC(VRAM_C_SUB_BG_0x06200000);

	// initialize background
	BACKGROUND.control[3] = BG_BMP16_256x256 | BG_BMP_BASE(0);
	BACKGROUND.bg3_rotation.hdx = 1 << 8;
	BACKGROUND.bg3_rotation.hdy = 0;
	BACKGROUND.bg3_rotation.vdx = 0;
	BACKGROUND.bg3_rotation.vdy = 1 << 8;
	// initialize sub-background
	BACKGROUND_SUB.control[3] = BG_BMP16_256x256 | BG_BMP_BASE(1);
	BACKGROUND_SUB.bg3_rotation.hdx = 1 << 8;
	BACKGROUND_SUB.bg3_rotation.hdy = 0;
	BACKGROUND_SUB.bg3_rotation.vdx = 0;
	BACKGROUND_SUB.bg3_rotation.vdy = 1 << 8;

	// paint the main screen red
	for(i = 0; i < 256*256; i++)
		videoBufferMain[i] = RGB15(31, 0, 0) | BIT(15);
	// paint the sub screen blue
	for(i = 0; i < 256*256; i++)
		videoBufferSub[i] = RGB15(0, 0, 31) | BIT(15);

	while(1) {
		swiWaitForVBlank();
	}

	return 0;
}
