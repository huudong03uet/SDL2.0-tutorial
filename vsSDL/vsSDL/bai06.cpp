#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gImg = NULL;

SDL_Surface* loadSurface(string path)
{
	SDL_Surface* optimizedSurface = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		cout << "error load surface";
	}
	else {
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
		if (optimizedSurface == NULL)
		{
			cout << "error optimized surface";
		}
		SDL_FreeSurface(loadedSurface);
	}
	return optimizedSurface;
}

bool loadMedia() {
	bool success = true;
	gImg = loadSurface("img/press.PNG");
	if (gImg == NULL)
	{
		cout << "error load media";
		success = false;
	}
	return success;
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
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				cout << "error img flags";
				success = false;
			}
			else
			{
				gScreenSurface = SDL_GetWindowSurface(gWindow);
			}
		}
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
int main(int argc, char* args[]) {
	init();
	if (loadMedia() == false)
	{
		cout << "error";
	}
	else {
		SDL_Rect stretchRect;
		stretchRect.x = 0;
		stretchRect.y = 0;
		stretchRect.h = SCREEN_HEIGHT;
		stretchRect.w = SCREEN_WIDTH;
		SDL_BlitSurface(gImg, NULL, gScreenSurface, &stretchRect);
		SDL_UpdateWindowSurface(gWindow);
		SDL_Delay(3000);
	}
	close();
	return 0;
}