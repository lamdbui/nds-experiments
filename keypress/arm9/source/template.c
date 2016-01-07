/*---------------------------------------------------------------------------------

	Simple console print demo
	-- dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	/*
	touchPosition touch;

	consoleDemoInit();  //setup the sub screen for printing

	iprintf("\n\n\tHello DS dev'rs\n");
	iprintf("\twww.drunkencoders.com\n");
	iprintf("\twww.devkitpro.org");

	while(1) {

		touchRead(&touch);
		iprintf("\x1b[10;0HTouch x = %04i, %04i\n", touch.rawx, touch.px);
		iprintf("Touch y = %04i, %04i\n", touch.rawy, touch.py);

		swiWaitForVBlank();
		scanKeys();
		if (keysDown()&KEY_START) break;
	}*/

	
	consoleDemoInit();

	//irqInit();
	//irqEnable(IRQ_VBLANK);

	while(1) {
		if(REG_KEYINPUT & KEY_A)
			iprintf("Key A is released");
		else
			iprintf("Key A is pressed");

		swiWaitForVBlank();
		consoleClear();
	}

		/*
	consoleDemoInit();
	
	while(1) {
		scanKeys();
		int held = keysHeld();

		if(held & KEY_A)
			printf("Key A is pressed\n");
		else
			printf("Key A is released\n");

		if(held & KEY_X)
			printf("Key X is pressed\n");
		else
			printf("Key X is released\n");

		if(held & KEY_TOUCH)
			printf("Touchpad is touched\n");
		else
			printf("Touchpad is not touched\n");

		swiWaitForVBlank();
		consoleClear();
	}*/

	return 0;
}
