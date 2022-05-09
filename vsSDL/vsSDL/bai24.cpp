#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <string>
#include <sstream>
#include <SDL_ttf.h>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



class Texture
{
public:
	Texture();
	~Texture();

	void free();
	bool loadFromFile(string path, SDL_Color textColor);
	void render(int x, int y, SDL_Rect* clip = NULL);

	int getWidth() const { return width; }
	int getHeight() const { return height; }
private:
	SDL_Texture* g_texture;
	int width;
	int height;
};

class Timer
{
public:
	Timer();

	void start();
	void stop();
	void pause();
	void unpause();

	Uint32 getTicks();

	bool isStarted();
	bool isPaused();

private:
	Uint32 startTicks;
	Uint32 pausedTicks;

	bool paused;
	bool started;
};

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;
Timer gTimer;


bool init();
SDL_Texture* loadTexture(string path);
bool loadMedia();
void close();

Texture gPauseTexture;
Texture gStartTexture;
Texture gTimeTexture;

int main(int argc, char* args[]) {
	init();

	if (loadMedia() == false) {
		return 1;
	}
	else {
		bool quit = false;
		SDL_Event e;
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderClear(gRenderer);

		Timer fpsTimer;
		int countedFrames = 0;
		fpsTimer.start();

		SDL_Color textColor = { 0,0,0, 255 };
		stringstream timeText;
		while (quit == false) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
			}
			float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
			if (avgFPS > 2000000)
			{
				avgFPS = 0;
			}
			SDL_RenderClear(gRenderer);

			timeText.str("");
			timeText << "Average Frames Per Second " << avgFPS;

			gTimeTexture.loadFromFile(timeText.str().c_str(), textColor);

			gTimeTexture.render(100, 100);

			SDL_RenderPresent(gRenderer);
			countedFrames++;
		}
	}

	close();
	return 0;
}
bool init()
{
	bool success = true;
	SDL_Init(SDL_INIT_VIDEO);

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

			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				cout << "error img init";
				success = false;
			}
		}
	}

	if (TTF_Init() == -1)
	{
		cout << "could not initialize";
		success = false;
	}
	return success;
}

bool loadMedia() {
	bool success = true;
	gFont = TTF_OpenFont("font/font.ttf", 15);
	if (gFont == NULL)
	{
		cout << "failed to load lazy font";
	}

	return success;
}

void close() {
	TTF_CloseFont(gFont);
	gFont = NULL;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}



void Texture::free()
{
	if (g_texture != NULL)
	{
		SDL_DestroyTexture(g_texture);
		g_texture = NULL;
		width = 0;
		height = 0;
	}
}

void Texture::render(int x, int y, SDL_Rect* clip)
{
	SDL_Rect renderQuad = { x, y, width, height };
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopy(gRenderer, g_texture, clip, &renderQuad);
}

bool Texture::loadFromFile(string path, SDL_Color textColor)
{
	free();

	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, path.c_str(), textColor);
	if (textSurface == NULL)
	{
		cout << "unable read the image";
	}
	else {

		g_texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (g_texture == NULL)
		{
			cout << "unable to create texture from surface";
		}
		else {
			width = textSurface->w;
			height = textSurface->h;
		}

		SDL_FreeSurface(textSurface);
	}
	return g_texture != NULL;

}

Texture::Texture()
{
	g_texture = NULL;
	width = 0;
	height = 0;
}

Texture::~Texture()
{
	free();
}

Timer::Timer()
{
	started = false;
	paused = false;
	startTicks = 0;
	pausedTicks = 0;
}

void Timer::start()
{
	started = true;
	paused = false;
	startTicks = SDL_GetTicks();
	pausedTicks = 0;
}

void Timer::stop()
{
	started = false;
	paused = false;
	startTicks = 0;
	pausedTicks = 0;
}

void Timer::pause()
{
	if (started && !paused)
	{
		paused = true;

		pausedTicks = SDL_GetTicks() - startTicks;
		startTicks = 0;
	}
}

void Timer::unpause()
{
	if (started && paused)
	{
		paused = false;
		startTicks = SDL_GetTicks() - pausedTicks;
		pausedTicks = 0;
	}
}

Uint32 Timer::getTicks()
{
	Uint32 time = 0;
	if (started)
	{
		if (paused)
		{
			time = pausedTicks;
		}
		else {
			time = SDL_GetTicks() - startTicks;
		}
	}
	return time;
}

bool Timer::isStarted()
{
	return started;
}

bool Timer::isPaused()
{
	return paused;
}