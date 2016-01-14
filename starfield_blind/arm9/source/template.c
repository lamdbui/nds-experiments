/***

Starfield demo

	We will draw a starfield using direct LCD (Franebuffer) access	

***/

#include <nds.h>

#define NUM_OF_STARS 	40
#define STAR_MAX_SPEED	4

#define SPACE_COLOR		RGB15(0,0,0)		// black
#define STAR_COLOR		RGB15(31,31,31)		// white

typedef struct {
	int x;
	int y;
	int speed;
	unsigned short color;

} Star;

Star stars[NUM_OF_STARS];

void ClearScreen() {
	int i;

	for(i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
		VRAM_A[i] = SPACE_COLOR;
}

void CreateStar(Star *star) {
	star->x = rand() % SCREEN_WIDTH;
	star->y = rand() % SCREEN_HEIGHT;
	star->speed = (rand() % STAR_MAX_SPEED) + 1;
	star->color = STAR_COLOR;
}

void InitStars(void) {
	int i;

	for(i = 0; i < NUM_OF_STARS; i++) {
		stars[i].x = rand() % SCREEN_WIDTH;
		stars[i].y = rand() % SCREEN_HEIGHT;
		stars[i].speed = (rand() % STAR_MAX_SPEED) + 1;
		stars[i].color = STAR_COLOR;
	}
}

void DrawStar(Star *star) {
	VRAM_A[(star->y * SCREEN_WIDTH) + star->x] = star->color;
}

void MoveStar(Star *star) {
	star->x += star->speed;

	// if it falls off the screen, we'll make a new star starting at the beginning
	if(star->x > SCREEN_WIDTH) {
		star->x = 0;
		star->y = rand() % SCREEN_HEIGHT;
		star->speed = (rand() % STAR_MAX_SPEED) + 1;
	}
}

void EraseStar(Star *star) {
	VRAM_A[(star->y * SCREEN_WIDTH) + star->x] = SPACE_COLOR;
}

int main() {

	int i;

	// enable interrupts
	irqInit();
	irqEnable(IRQ_VBLANK);

	// select our video mode (frame buffer) via control registers
	videoSetMode(MODE_FB0);
	// configure our VRAM to use LCD (frame buffer) mode
	vramSetBankA(VRAM_A_LCD);

	// setup the initial Starfield
	ClearScreen();
	InitStars();

	while(1) {

		for(i = 0; i < NUM_OF_STARS; i++) {
			EraseStar(&stars[i]);
			MoveStar(&stars[i]);
			DrawStar(&stars[i]);
		}

		swiWaitForVBlank();
	}

	return 0;
}