#include <SDL.h>
#include <iostream>
using namespace std;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gImg = NULL;
bool init();
bool loadMedia();
void close();
int main(int argc, char* args[]) {
	init();
	if (loadMedia() == false) {
		return 1;
	}
	else {
		SDL_BlitSurface(gImg, NULL, gScreenSurface, NULL);
		SDL_UpdateWindowSurface(gWindow);
		SDL_Delay(3000);
	}
	close();
	return 0;
}
bool init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		success = false;
		cout << "error1";
	}
	else {
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			success = false;
			cout << "error2";
		}
		else {
			gScreenSurface = SDL_GetWindowSurface(gWindow);
			SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 123, 123, 123));
			SDL_UpdateWindowSurface(gWindow);
		}
	}
	return success;
}

bool loadMedia() {
	bool success = true;
	gImg = SDL_LoadBMP("img/press.bmp");
	if (gImg == NULL) {
		success = false;
		cout << "error3";
	}
	return success;
}

void close() {
	SDL_FreeSurface(gImg);
	gImg = NULL;
	
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

