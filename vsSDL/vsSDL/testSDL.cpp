#include <SDL.h>
#include <iostream>
#include "Painter.h"
#include "function.h"
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "SDL tutorial";

SDL_Window* gWindow;
SDL_Renderer* gRenderer;

const SDL_Rect  SCREEN_RECT = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };


void init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "error sdl init";
		exit(1);
	}
	gWindow = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		cout << "error create window"; exit(1);
	}
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == NULL)
	{
		cout << "error create renderer"; exit(1);
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);	
}

void close()
{
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

void waitUntilKeyPressed()
{
	SDL_Event e;
	while (true)
	{
		if (SDL_WaitEvent(&e) != 0 &&
			(e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
			return;
		SDL_Delay(100);
	}
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	init();
	Painter painter(gWindow, gRenderer);
	
	painter.clearWithBackGroundColor(WHITE_COLOR);

	drawSnowFlake(painter);

	SDL_RenderPresent(gRenderer);

	waitUntilKeyPressed();
	close();
	return 0;
}