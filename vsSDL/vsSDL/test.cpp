#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <string>
#include <SDL_mixer.h>
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

Mix_Music* gMusic = NULL;

Mix_Chunk* gScratch = NULL;
Mix_Chunk* gHigh = NULL;
Mix_Chunk* gMedium = NULL;
Mix_Chunk* gLow = NULL;


int main(int argc, char* args[]) {
	init();
	if (loadMedia() == false) {
		return 1;
	}
	else {
		bool quit = false;
		SDL_Event e;
		while (quit == false) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				if (e.type == SDL_KEYDOWN)
				{
					switch (e.key.keysym.sym)
					{
					case SDLK_1:
						Mix_PlayChannel(-1, gHigh, 0); break;
					case SDLK_2:
						Mix_PlayChannel(-1, gMedium, 0); break;
					case SDLK_3:
						Mix_PlayChannel(-1, gLow, 0); break;
					case SDLK_4:
						Mix_PlayChannel(-1, gScratch, 0); break;
					case SDLK_9:

						if (Mix_PlayingMusic() == 0)
						{
							Mix_PlayMusic(gMusic, -1);
						}
						else
						{
							if (Mix_PausedMusic() == 1)
							{
								Mix_ResumeMusic();
							}
							else
							{
								Mix_PauseMusic();
							}
						}
						break;
					case SDLK_0:
						Mix_HaltMusic(); break;
					}
				}
			}
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
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		success = false;
		cout << "error1";
	}
	else {
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			success = false;
			cout << "error2";
		}
		else {
			//gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer != NULL) {
				success = false;

				int imgFlags = IMG_INIT_PNG;

				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					cout << "error img init";
					success = false;
				}
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					cout << "error mixer";
					success = false;
				}

			}
		}
	}
	return success;
}

bool loadMedia() {
	bool success = true;
	gTexture = loadTexture("img/prompt.png");
	if (gTexture == NULL) {
		success = false;
		cout << "error3";
	}
	gMusic = Mix_LoadMUS("sound/beat.wav");
	gScratch = Mix_LoadWAV("sound/scratch.wav");
	gHigh = Mix_LoadWAV("sound/high.wav");
	gMedium = Mix_LoadWAV("sound/medium.wav");
	gLow = Mix_LoadWAV("sound/low.wav");


	return success;
}

void close() {
	Mix_FreeChunk(gScratch);
	Mix_FreeChunk(gHigh);
	Mix_FreeChunk(gMedium);
	Mix_FreeChunk(gLow);

	Mix_FreeMusic(gMusic);
	gMusic = NULL;

	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
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