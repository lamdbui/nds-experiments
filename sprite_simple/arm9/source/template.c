/*---------------------------------------------------------------------------------

	Simple sprite demo
	- 1D palette-based sprite on both screens using the touchpad for location
	- We generate the image data(tile map/palette) on our own
	- OAM is used to manage the sprite data (max of 128 sprites - 32 can be affine-transformed)
	BONUS:
	- Move the sprite around with the keypad
	- Add some rotation with the L/R flippers - sub is double speed of the main

	QUESTIONS:
	- The movement spills off the screen and continues on the other side. There must be some overflow.
	  What size are ints on the NDS?

---------------------------------------------------------------------------------*/
#include <nds.h>

#define MOVEMENT_RATE			2
#define ROTATION_RATE			2
#define SPRITE_DIMENSION_SIZE	16

int main() {

	int i;
	int currentPosX = (SCREEN_WIDTH - (SPRITE_DIMENSION_SIZE/2)) / 2;
	int currentPosY = (SCREEN_HEIGHT - (SPRITE_DIMENSION_SIZE/2)) / 2;
	int angle = degreesToAngle(0);
	touchPosition touch;
	u16 *gfx;
	u16 *gfxSub;

	// setup our graphics modes
	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	// configure our VRAM banks to store the sprite data
	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	// initialize oam
	oamInit(&oamMain, SpriteMapping_1D_32, false);
	oamInit(&oamSub, SpriteMapping_1D_32, false);

	// allocate our graphics memory
	gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
	gfxSub = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);

	// generate our sprite tile mapping in memory
	// in this case, we're using palette index 1 for both the main and sub screens
	for(i = 0; i < 16 * 16 / 2; i++) {
		gfx[i] = 1 | 1 << 8;
		gfxSub[i] = 1 | 1 << 8;
	}

	// create our sprite palette in memory
	SPRITE_PALETTE[1] = RGB15(31,0,0);
	SPRITE_PALETTE_SUB[1] = RGB15(0,0,31);

	while(1) {

		// check our touch location
		scanKeys();
		if(keysHeld() & KEY_TOUCH) {
			touchRead(&touch);
			currentPosX = touch.px;
			currentPosY = touch.py;
		}
		else if(keysHeld() & KEY_UP) {
			currentPosY -= MOVEMENT_RATE;
		}
		else if(keysHeld() & KEY_DOWN) {
			currentPosY += MOVEMENT_RATE; 
		}
		else if(keysHeld() & KEY_LEFT) {
			currentPosX -= MOVEMENT_RATE;
		}
		else if(keysHeld() & KEY_RIGHT) {
			currentPosX += MOVEMENT_RATE;
		}
		else if(keysHeld() & KEY_L) {
			angle -= degreesToAngle(ROTATION_RATE);
		}
		else if(keysHeld() & KEY_R) {
			angle += degreesToAngle(ROTATION_RATE);
		}

		oamRotateScale(&oamMain,
			0,							// rotation ID (we can have up to 32 different affine matrices!)
			angle,						// angle of the rotation
			intToFixed(1, 8),			// inverse scale factor of X
			intToFixed(1, 8)			// inverse scale factor of Y
			);

		oamRotateScale(&oamSub,
			0,							// rotation ID (we can have up to 32 different affine matrices!)
			angle * 2,					// angle of the rotation (double the main screen[0])
			intToFixed(1, 8),			// inverse scale factor of X
			intToFixed(1, 8)			// inverse scale factor of Y
			);

		// set our new OAM data
		oamSet(&oamMain,
			0,							// setting the OAM ID
			currentPosX,				// X location of the sprite
			currentPosY,				// Y location of the sprite
			0,							// sprite priority
			0,							// palette index, or alpha value of using a bitmap sprite
			SpriteSize_16x16, 			// size of the sprite
			SpriteColorFormat_256Color,	// color format of the sprite
			gfx,						// address of the sprite data
			0,							// affine index of sprite (< 0 or > 31 means unrotated)
			true,						// double size for rotation? (so it doesn't get clipped)
			false,						// hide the sprite?
			false,						// V-flip the sprite?
			false,						// H-flip the sprite?
			false						// apply mosaic effect?
			);

		oamSet(&oamSub,
			0,							// setting the OAM ID
			currentPosX,				// X location of the sprite
			currentPosY,				// Y location of the sprite
			0,							// sprite priority
			0,							// palette index, or alpha value of using a bitmap sprite
			SpriteSize_16x16, 			// size of the sprite
			SpriteColorFormat_256Color,	// color format of the sprite
			gfxSub,						// address of the sprite data
			0,							// affine index of sprite (< 0 or > 31 means unrotated)
			true,						// double size for rotation? (so it doesn't get clipped)
			false,						// hide the sprite?
			false,						// V-flip the sprite?
			false,						// H-flip the sprite?
			false						// apply mosaic effect?
			);

		swiWaitForVBlank();

		// update(copy) our data over from our buffers to actual OAM memory location
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}


	return 0;
}

