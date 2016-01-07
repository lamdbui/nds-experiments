/*---------------------------------------------------------------------------------

	Simple console print demo
	-- dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	int i;

	consoleDemoInit();
	// set framebuffer Mode 0
	videoSetMode(MODE_FB0);
	// enable VRAM A for writing by the CPU and use as a framebuffer by video hardware
	vramSetBankA(VRAM_A_LCD);

	while(1) { 

		//u16 color = RGB15(31, 0, 0);	// red
		u16 color = 0;
		//color |= 31;
		//color |= (31 << 5);
		scanKeys();
		int held = keysHeld();

		if(held & KEY_B)
			color |= 31;
		if(held & KEY_A)
			//color = RGB15(0, 31, 0);	// green
			color |= (31 << 5);
		if(held & KEY_X)
			//color = RGB15(0, 0, 31);
			color |= (31 << 10);

		swiWaitForVBlank();

		for(i = 0; i < 256*192 /*screen resolution*/; i++)
			VRAM_A[i] = color;
	}

	return 0;
}
