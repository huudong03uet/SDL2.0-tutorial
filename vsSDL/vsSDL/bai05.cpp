#include <SDL.h>
#include <iostream>
using namespace std;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gImg = NULL;
bool init();
SDL_Surface* loadedSurface(string path);
bool loadMedia();

void close();
int main(int argc, char* args[]) {
	init();
	if (loadMedia() == false) {
		return 1;
	}
	else {
		bool quit = false;
		SDL_Event e;
		SDL_Rect stretchRect;
		
		double i = 0;
		while (quit == false) {
			if (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
			}
			//SDL_BlitSurface(gImg, NULL, gScreenSurface, NULL);
			i += 0.002;
			stretchRect.x = 0;
			stretchRect.y = 0;
			stretchRect.w = i * SCREEN_WIDTH;
			stretchRect.h = i * SCREEN_HEIGHT;
			SDL_Delay(10);
			SDL_BlitScaled(gImg, NULL, gScreenSurface, &stretchRect);
			SDL_UpdateWindowSurface(gWindow);

		}
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
	gImg = loadedSurface("img/press.bmp");
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

SDL_Surface* loadedSurface(string path) {
	SDL_Surface* optimizedSurface = NULL;
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL) {
		cout << "error loaded surface";
	}
	else {
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
		if (optimizedSurface == NULL)
		{
			cout << "error optimizedSurface";
		}
		SDL_FreeSurface(loadedSurface);
	}
	return optimizedSurface;
}

