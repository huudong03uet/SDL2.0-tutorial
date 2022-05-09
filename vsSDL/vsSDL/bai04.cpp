#include <SDL.h>
#include <iostream>
using namespace std;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gKeyPressSurface[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface* gCurrentSurface = NULL;

bool init();
SDL_Surface* loadSurface(string path);
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
		while (quit == false) {
			if (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				else if (e.type == SDL_KEYDOWN)
				{
					switch (e.key.keysym.sym)
					{
					case SDLK_UP:
						gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_UP]; break;
					case SDLK_DOWN:
						gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DOWN]; break;
					case SDLK_RIGHT:
						gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT]; break;
					case SDLK_LEFT:
						gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_LEFT]; break;
					default:
						gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];
						break;
					}
				}
			}
			SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
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

	gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("img/press.bmp");
	gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT] = loadSurface("img/right.bmp");
	gKeyPressSurface[KEY_PRESS_SURFACE_DOWN] = loadSurface("img/down.bmp");
	gKeyPressSurface[KEY_PRESS_SURFACE_LEFT] = loadSurface("img/left.bmp");
	gKeyPressSurface[KEY_PRESS_SURFACE_UP] = loadSurface("img/up.bmp");

	if (gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT] == NULL ||
		 gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT] == NULL ||
		 gKeyPressSurface[KEY_PRESS_SURFACE_DOWN] == NULL ||
		 gKeyPressSurface[KEY_PRESS_SURFACE_LEFT] == NULL ||
		 gKeyPressSurface[KEY_PRESS_SURFACE_UP] == NULL)
	{
		cout << "error loadMedia";
		success = false;
	}
	return success;
}

void close() {

	for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++)
	{
		SDL_FreeSurface(gKeyPressSurface[i]);
		gKeyPressSurface[i] = NULL;
	}

	SDL_FreeSurface(gCurrentSurface);
	gCurrentSurface = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

SDL_Surface* loadSurface(string path)
{
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL)
	{
		cout << "unable load image " << path;
	}
	return loadedSurface;
}