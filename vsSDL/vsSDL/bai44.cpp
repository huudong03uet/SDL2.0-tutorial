#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <string>
#include <sstream>
#include <SDL_ttf.h>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;


class Texture
{
public:
	Texture();
	~Texture();

	void free();
	bool loadFromFile(string path);
	void render(int x, int y, SDL_Rect* clip = NULL);

	int getWidth() const { return width; }
	int getHeight() const { return height; }
private:
	SDL_Texture* g_texture;
	int width;
	int height;
	void* pixels_;
	int pitch_;
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

class Dot
{
public:
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;

	static const int DOT_VEL = 640;

	Dot();
	void handleEvent(SDL_Event& e);
	void move(float timeStep);

	void render();

private:
	float posX, posY;
	float velX, velY;
};

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;
Timer gTimer;

Texture gDotTexture;

bool init();
SDL_Texture* loadTexture(string path);
bool loadMedia();
void close();

Dot dot;
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
				dot.handleEvent(e);

			}
			float timeStep = gTimer.getTicks() / 1000.f;
			dot.move(timeStep);
			gTimer.start();
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderClear(gRenderer);
			dot.render();
			SDL_RenderPresent(gRenderer);
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

bool loadMedia()
{

	bool success = true;

	if (!gDotTexture.loadFromFile("img/dot.bmp"))
	{
		printf("Failed to load dot texture!\n");
		success = false;
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

bool Texture::loadFromFile(string path)
{
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

		SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGBA8888, 0);
		if (formattedSurface == NULL)
		{
			printf("Unable to convert loaded surface to display format! %s\n", SDL_GetError());
		}
		else
		{
			//Create blank streamable texture
			newTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);
			if (newTexture == NULL)
			{
				printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
			}
			else
			{

				SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);

				SDL_LockTexture(newTexture, &formattedSurface->clip_rect, &pixels_, &pitch_);

				memcpy(pixels_, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);

				width = formattedSurface->w;
				height = formattedSurface->h;

				//Get pixel data in editable format
				Uint32* pixels = (Uint32*)pixels_;
				int pixelCount = (pitch_ / 4) * height;

				//Map colors				
				Uint32 colorKey = SDL_MapRGB(formattedSurface->format, 0, 0xFF, 0xFF);
				Uint32 transparent = SDL_MapRGBA(formattedSurface->format, 0x00, 0xFF, 0xFF, 0x00);

				for (int i = 0; i < pixelCount; ++i)
				{
					if (pixels[i] == colorKey)
					{
						pixels[i] = transparent;
					}
				}

				SDL_UnlockTexture(newTexture);
				pixels_ = NULL;
			}

			SDL_FreeSurface(formattedSurface);
		}

		SDL_FreeSurface(loadedSurface);
	g_texture = newTexture;
	return g_texture != NULL;
}

Texture::Texture()
{
	g_texture = NULL;
	width = 0;
	height = 0;
	pixels_ = 0;
	pitch_ = NULL;
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

void Dot::move(float timeStep)
{
	posX += velX * timeStep;
	if (posX < 0)
	{
		posX = 0;
	}
	else if (posX > SCREEN_WIDTH - DOT_WIDTH)
	{
		posX = SCREEN_WIDTH - DOT_WIDTH;
	}
	posY += velY * timeStep;
	if (posY < 0)
	{
		posY = 0;
	}
	else if (posY > SCREEN_HEIGHT - DOT_HEIGHT)
	{
		posY = SCREEN_HEIGHT - DOT_HEIGHT;
	}
}

void Dot::render()
{
	gDotTexture.render((int)posX, (int)posY);
}

Dot::Dot()
{
	posX = 0;
	posY = 0;
	velX = 0;
	velY = 0;
}

void Dot::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: velY -= DOT_VEL; break;
		case SDLK_DOWN: velY += DOT_VEL; break;
		case SDLK_LEFT: velX -= DOT_VEL; break;
		case SDLK_RIGHT: velX += DOT_VEL; break;
		default:
			break;
		}
	}
	if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: velY += DOT_VEL; break;
		case SDLK_DOWN: velY -= DOT_VEL; break;
		case SDLK_LEFT: velX += DOT_VEL; break;
		case SDLK_RIGHT: velX -= DOT_VEL; break;
		default:
			break;
		}
	}
}