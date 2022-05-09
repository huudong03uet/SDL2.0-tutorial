#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <string>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;

bool init();
SDL_Texture* loadTexture(string path);
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
			}
			SDL_RenderClear(gRenderer);
			SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
			SDL_RenderPresent(gRenderer);
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
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL) {
				cout << "error render";
				success = false;
				SDL_SetRenderDrawColor(gRenderer, 123, 123, 13, 123);
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					cout << "error img init";
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadMedia() {
	bool success = true;
	gTexture = loadTexture("img/press.PNG");
	if (gTexture == NULL) {
		success = false;
		cout << "error3";
	}
	return success;
}

void close() {
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}



SDL_Texture* loadTexture(string path)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		cout << "error loaded surface";
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			cout << "not create texture";
		}
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}
