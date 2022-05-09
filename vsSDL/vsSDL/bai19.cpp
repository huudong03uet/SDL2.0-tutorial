#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <string>
using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int JOYSTICK_DEAD_ZONE = 8000;
SDL_Joystick* gGameController = NULL;

class Texture
{
public:
	Texture();
	~Texture();

	void free();
	bool loadFromFile(string path);
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip = SDL_FLIP_NONE);

	int getWidth() const { return width; }
	int getHeight() const { return height; }
private:
	SDL_Texture* g_texture;
	int width;
	int height;
};


SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

bool init();
SDL_Texture* loadTexture(string path);
bool loadMedia();
void close();

Texture gTexture;
SDL_Rect gRectTexture[4];
int main(int argc, char* args[]) {
	init();

	if (loadMedia() == false) {
		return 1;
	}
	else {
		bool quit = false;
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 1);
		SDL_RenderClear(gRenderer);
		SDL_Event e;
		int xDir = 0;
		int yDir = 0;
		while (quit == false) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				else if(e.type == SDL_JOYAXISMOTION)
				{
					if (e.jaxis.which == 0)
					{
						if (e.jaxis.value < -JOYSTICK_DEAD_ZONE)
						{
							xDir = -1;
						}
						else if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
						{
							xDir = 1;
						}
						else
						{
							xDir = 0;
						}
					}
					else if (e.jaxis.axis == 1)
					{
						if (e.jaxis.value < -JOYSTICK_DEAD_ZONE)
						{
							yDir = -1;
						}
						else if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
						{
							yDir = 1;
						}
						else {
							yDir = 0;
						}
					}
				}
			}
			double joystickAngle = atan2((double)yDir, (double)xDir) * (180.0 / M_PI);
			if (xDir == 0 && yDir == 0)
			{
				joystickAngle = 0;
			}
			gTexture.render((SCREEN_WIDTH - gTexture.getWidth()) / 2, (SCREEN_HEIGHT - gTexture.getHeight()) / 2, NULL, joystickAngle);
			SDL_RenderPresent(gRenderer);
		}
	}

	close();
	return 0;
}
bool init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		success = false;
		cout << "error1";

	}
	else {
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			cout << "warning";
		}
		if (SDL_NumJoysticks() < 1)
		{
			cout << "warning";
		}
		else
		{
			gGameController = SDL_JoystickOpen(0);
			if (gGameController == NULL)
			{
				cout << "warning";
			}
		}
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
	if (!gTexture.loadFromFile("img/arrow.png"))
	{
		cout << "failed to load";
		success = false;
	}
	return success;
}

void close() {
	gTexture.free();

	SDL_JoystickClose(gGameController);
	gGameController = NULL;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

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

void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = { x, y, width, height };
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx(gRenderer, g_texture, clip, &renderQuad, angle, center,flip);
}

bool Texture::loadFromFile(string path)
{
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		cout << "unable read the image";
	}
	else {
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			cout << "unable to create texture from surface";

		}
		else {
			width = loadedSurface->w;
			height = loadedSurface->h;
		}

		SDL_FreeSurface(loadedSurface);
	}
	g_texture = newTexture;
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