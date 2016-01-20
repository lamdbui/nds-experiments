/*** 

	Starfield demo
	- Simulate a horizontal scrolling starfield using a framebuffer (no background tiles or sprites)
	- Stars are tracked as an array of Star
	- We will do the logic and drawing during the Vblank period

***/

#include <nds.h>

#define NUM_OF_STARS	40
#define MAX_STAR_SPEED	4
#define SPACE_COLOR		RGB15(0,0,0)	// black
#define STAR_COLOR		RGB15(31,31,31)	// white

// our Star object
typedef struct {
	int x;
	int y;
	int speed;
	unsigned short color;
} Star;

// our Stars
Star stars[NUM_OF_STARS];

// wipe everything to make sure we're drawing on an empty screen
void ClearScreen() {
	int i;

	for(i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		VRAM_A[i] = SPACE_COLOR;
	}
}

// initialize our array of Stars
void InitStars() {
	int i;

	for(i = 0; i < NUM_OF_STARS; i++) {
		stars[i].x = rand() % SCREEN_WIDTH;
		stars[i].y = rand() % SCREEN_HEIGHT;
		stars[i].speed = (rand() % MAX_STAR_SPEED) + 1;	// because the range is 1-4. We don't want stars that are stopped
		stars[i].color = STAR_COLOR;
	}
}

// we can "erase" it, but just setting it to the SPACE_COLOR
void EraseStar(Star *star) {
	VRAM_A[(star->y * SCREEN_WIDTH) + star->x] = SPACE_COLOR;
}

void MoveStar(Star *star) {
	star->x += star->speed;

	// checks to see if it falls off screen. If it does, we create a new star
	if(star->x > SCREEN_WIDTH) {
		star->x = 0;
		star->y = rand() % SCREEN_HEIGHT;
		star->speed = (rand() % MAX_STAR_SPEED) + 1;
	}
}

void DrawStar(Star *star) {
	VRAM_A[(star->y * SCREEN_WIDTH) + star->x] = star->color;
}

int main() {

	int i;

	// enable the Vblank interrupt
	irqInit();
	irqEnable(IRQ_VBLANK);

	// setup our video mode
	REG_DISPCNT = MODE_FB0;
	// enable our Vram (only what we need)
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_LCD;
	
	// initialize our starfield
	ClearScreen();
	InitStars();

	// do logic and drawing for our starfield during Vblank
	while(1) {
		for(i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
			EraseStar(&stars[i]);
			MoveStar(&stars[i]);
			DrawStar(&stars[i]);
		}

		// make sure we pause for the whole VBlank period before we end
		swiWaitForVBlank();
	}

	return 0;
}