/*---------------------------------------------------------------------------------

	Starfield demo

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

#define NUM_OF_STARS 40

#define STAR_COLOR 	RGB15(31, 31, 31)
#define SPACE_COLOR	RGB15(0, 0, 0)

typedef struct {
	int x;
	int y;
	int speed;
	unsigned short color;
} Star;

Star stars[NUM_OF_STARS];

void ClearScreen(void) {
	int i;

	for(i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
		VRAM_A[i] = RGB15(0, 0, 0);	// blacked out screen
}

// setup Stars and place them randomly on the screen
void InitStars(void) {
	int i;

	for(i = 0; i < NUM_OF_STARS; i++) {
		stars[i].x = rand() % SCREEN_WIDTH;
		stars[i].y = rand() % SCREEN_HEIGHT;
		stars[i].speed = (rand() % 4) + 1; 	// between [1-4] to keep it moving
		stars[i].color = STAR_COLOR;
		//stars[i].color = RGB15(31, 31, 31);	// white stars
	}
}

// erase the Star if it's no longer visible/needed
// we'll just set this tothe background color
void EraseStar(Star *star) {
	VRAM_A[(star->y * SCREEN_WIDTH) + star->x] = SPACE_COLOR;
}

void DrawStar(Star *star) {
	VRAM_A[(star->y * SCREEN_WIDTH) + star->x] = star->color;
}

void MoveStar(Star *star) {
	star->x += star->speed;

	// check to see if it goes off the screen
	// if it goes off, we give it some new values so it looks like a new star
	if(star->x > SCREEN_WIDTH) {
		star->x = 0;
		star->y = rand() % SCREEN_HEIGHT;
		star->speed = (rand() % 4) + 1;
	}
}

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	int i;

	irqInit();
	irqEnable(IRQ_VBLANK);

	videoSetMode(MODE_FB0);
	vramSetBankA(VRAM_A_LCD);

	ClearScreen();
	InitStars();

	while(1) {
		swiWaitForVBlank();

		for(i = 0; i < NUM_OF_STARS; i++) {
			EraseStar(&stars[i]);
			MoveStar(&stars[i]);
			DrawStar(&stars[i]);
		}
	}

	return 0;
}
