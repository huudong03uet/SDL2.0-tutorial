#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <string>
#include <SDL_ttf.h>
#include <sstream>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Texture* gTexture = NULL;
SDL_Rect renderQuadTexture;

TTF_Font* gFont = NULL;

SDL_Texture* gTimeText = NULL;
SDL_Rect renderQuadTimeText;

SDL_Texture* loadFromRenderText(string textureText, SDL_Color textColor, bool isTexture);
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
		SDL_Color textColor = { 0,0,0,255 };
		Uint32 startTime = 0;
		stringstream timeText;
		
		while (quit == false) {
			if (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
				{
					startTime = SDL_GetTicks();
				}
			}
			timeText.str("");
			timeText << "Milliseconds since start time " << (SDL_GetTicks() - startTime) / 1000 << "s";

			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderClear(gRenderer);

			gTimeText = loadFromRenderText(timeText.str().c_str(), { 0, 0, 0, 255 }, 0);
			gTexture = loadFromRenderText("Press enter to reset Start Time", { 0, 0, 0, 255 }, 1);	

			SDL_RenderCopy(gRenderer, gTexture, NULL, &renderQuadTexture);
			SDL_RenderCopy(gRenderer, gTimeText, NULL, &renderQuadTimeText);			

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
			}
			else {
				SDL_SetRenderDrawColor(gRenderer, 123, 123, 13, 123);
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					cout << "error img init";
					success = false;
				}
				if (TTF_Init() == -1)
				{
					cout << "error";
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadMedia() {
	bool success = true;
	gFont = TTF_OpenFont("font/lazy.ttf", 30);
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

SDL_Texture* loadFromRenderText(string textureText, SDL_Color textColor,bool isTexture)
{
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	SDL_Texture* g_texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
	if (isTexture == 1) {
		renderQuadTexture.x = (SCREEN_WIDTH - textSurface->w) / 2;
		renderQuadTexture.y = 0;
		renderQuadTexture.w = textSurface->w;
		renderQuadTexture.h = textSurface->h;
	}
	else {
		renderQuadTimeText.x = 50;
		renderQuadTimeText.y = SCREEN_HEIGHT / 2;
		renderQuadTimeText.w = textSurface->w;
		renderQuadTimeText.h = textSurface->h;
	}
	if (g_texture == NULL) {
		cout << "unable to create texture";
	}
	else {

	}
	SDL_FreeSurface(textSurface);
	return g_texture;
}