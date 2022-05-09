#include <SDL.h>
#include <iostream>
#include <SDL_image.h>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "SDL tutorial";

SDL_Window* gWindow;
SDL_Renderer* gRenderer;
SDL_Texture* gTexture;

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
	SDL_Texture* texture = NULL;
	SDL_DestroyTexture(texture);

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

SDL_Texture* loadTexture(string path)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		cout << "Unable to load image" << path << "SDL_image error: "
			<< IMG_GetError() << endl;
	}
	else {
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			cout << "Unable to create texture from " << path << "SDL Error: "
				<< SDL_GetError() << endl;
			SDL_FreeSurface(loadedSurface);
		}
		return newTexture;
	}
}

bool createImage(SDL_Texture* texture)
{
	if (texture == NULL) return false;
	SDL_RenderCopy(gRenderer, texture, NULL, NULL);
	return true;
}

int main(int argc, char* argv[])
{

	init();

	gTexture = loadTexture("img/press.PNG");
	createImage(gTexture);
	SDL_RenderPresent(gRenderer);
	waitUntilKeyPressed();

	close();
	return 0;
}