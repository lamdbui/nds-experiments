/*---------------------------------------------------------------------------------

	simple sprite demo using an F-18 to move around
	- Load basic sprite
	- Add ability to rotate(L/R)
	- Add ability to move around(D-pad)
	BONUS:
	[TODO] Figure out what to do to add a thurst animation
	[TODO] Try adding a space/sky background
	[TODO] Make space/sky background move appropriately in relation to direction of the ship
	[TODO] Use the sprite sheet instead
	[TODO] Switch to different sprite with yaw and bank (D-pad)

---------------------------------------------------------------------------------*/
#include <nds.h>

#include "F-18-2-0.h"

// this is padding to account for the empty space in the actual sprite graphic
#define SHIP_SPRITE_PAD 4

#define SHIP_START_POS_X 100
#define SHIP_START_POS_Y 50

#define ROTATION_RATE 2
#define BANK_RATE 2
#define THRUST_RATE 2

int main() {

	u16 *mainSpriteMem;
	int shipAngle = 0;
	//int shipThrust = 0;
	int shipPosX = SHIP_START_POS_X;
	int shipPosY = SHIP_START_POS_Y;
	int keys;

	// setup our interrupts to draw at the appropriate time
	irqInit();
	irqEnable(IRQ_VBLANK);

	// setup our video mode
	videoSetMode(MODE_0_2D);

	// select what VRAM banks we want to use for sprite data
	vramSetBankA(VRAM_A_MAIN_SPRITE);

	// initialize our OAM
	oamInit(&oamMain, SpriteMapping_1D_32, false);

	// reserve our OAM memory for the sprites
	mainSpriteMem = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_16Color);
	
	// fill our reserved OAM memory with the actual sprite maps
	DC_FlushRange(mainSpriteMem, F_18_2_0TilesLen);
	dmaCopy(F_18_2_0Tiles, mainSpriteMem, F_18_2_0TilesLen);

	// load our palette data
	DC_FlushRange(SPRITE_PALETTE, F_18_2_0PalLen);
	dmaCopy(F_18_2_0Pal, SPRITE_PALETTE, F_18_2_0PalLen);

	while(1) {

		// scan for input
		scanKeys();
		keys = keysHeld();

		// handle our input
		if(keys & KEY_L) {
			shipAngle += ROTATION_RATE;
			if(shipAngle > 360) {
				shipAngle -= 360;
			}
		}
		if(keys & KEY_R) {
			shipAngle -= ROTATION_RATE;
			if(shipAngle < 0) {
				shipAngle = 360 - shipAngle;
			}
		}
		if(keys & KEY_LEFT) {
			shipPosX -= BANK_RATE;
			if(shipPosX < -(32/*sprite size*//2) - SHIP_SPRITE_PAD) {
				shipPosX = -(32/*sprite size*//2) - SHIP_SPRITE_PAD;
			}
		}
		if(keys & KEY_RIGHT) {
			shipPosX += BANK_RATE;
			if(shipPosX > SCREEN_WIDTH - 32 - (32/*sprite size*//2) + SHIP_SPRITE_PAD) {
				shipPosX = SCREEN_WIDTH - 32 - (32/*sprite size*//2) + SHIP_SPRITE_PAD;
			}
		}
		if(keys & KEY_UP) {
			shipPosY -= THRUST_RATE;
			if(shipPosY < (32/*sprite size*//2)) {
				shipPosY = (32/*sprite size*//2);
			}
		}
		if(keys & KEY_DOWN) {
			shipPosY += THRUST_RATE;
			if(shipPosY > SCREEN_HEIGHT - (32/*sprite size*//2)) {
				shipPosY = SCREEN_HEIGHT - (32/*sprite size*//2);
			}
		}

		// setup our sprite affine matrix
		oamRotateScale(&oamMain,		// main graphics engine context
			0,							// set rotation matrix index (we can have up to 32)			
			degreesToAngle(shipAngle),	// convert to binary radians (-32768 to 32767) for nds
			(1 << 8),					// 8.8 fixed point inverse scale factor X
			(1 << 8)					// 8.8 fixed point inverse scale factor Y
			);

		// configure our sprite OAM data
		oamSet(&oamMain,				// main graphics engine context
			0,           				// oam index (0 to 127)  
			shipPosX, shipPosY,   		// x and y pixel location of the sprite
			0,                    		// priority, lower renders last (on top)
			0,					  		// this is the palette index if multiple palettes or the alpha value if bmp sprite	
			SpriteSize_32x32,     
			SpriteColorFormat_16Color, 
			mainSpriteMem,              //pointer to the loaded graphics
			0,                  		//sprite rotation/scale matrix index 
			true,               		//double the size when rotating?
			false,						//hide the sprite?
			false, false, 				//vflip, hflip
			false						//apply mosaic
			);  

		swiWaitForVBlank();

		// update the OAM with our changes
		oamUpdate(&oamMain);
	}

	return 0;
}
