#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <string>
#include <SDL_ttf.h>
using namespace std;

#define COLOR_KEY_R_ 0
#define COLOR_KEY_G_ 255
#define COLOR_KEY_B_ 255

const int WALKING_ANIMATION_FRAMES = 4;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;

enum SDL_SCANCODE_
{
	SDL_SCANCODE_DEFAULT_ = 0,
	SDL_SCANCODE_UP_ = 1,
	SDL_SCANCODE_DOWN_ = 2,
	SDL_SCANCODE_LEFT_ = 3,
	SDL_SCANCODE_RIGHT_ = 4,
	SDL_SCANCODE_TOTAL_ = 5
};

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
};


SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

bool init();
SDL_Texture* loadTexture(string path);
bool loadMedia();
void close();

Texture gTexture;
Texture gDirectTexture[SDL_SCANCODE_TOTAL_];

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

		while (quit == false) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
				if (currentKeyStates[SDL_SCANCODE_UP])
				{
					gTexture = gDirectTexture[SDL_SCANCODE_UP_];
				}
				else if (currentKeyStates[SDL_SCANCODE_DOWN])
				{
					gTexture = gDirectTexture[SDL_SCANCODE_DOWN_];
				}
				else if (currentKeyStates[SDL_SCANCODE_LEFT])
				{
					gTexture = gDirectTexture[SDL_SCANCODE_LEFT_];
				}
				else if (currentKeyStates[SDL_SCANCODE_RIGHT])
				{
					gTexture = gDirectTexture[SDL_SCANCODE_RIGHT_];
				}
				else
				{
					gTexture = gDirectTexture[SDL_SCANCODE_DEFAULT_];
				}
			}
			gTexture.render(0, 0);
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
			SDL_SetRenderDrawColor(gRenderer, 123, 123, 13, 123);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				cout << "error img init";
				success = false;
			}
		}
	}
	return success;
}

bool loadMedia() {
	bool success = true;
	gDirectTexture[SDL_SCANCODE_UP_].loadFromFile("img/up.png");
	gDirectTexture[SDL_SCANCODE_DOWN_].loadFromFile("img/down.png");
	gDirectTexture[SDL_SCANCODE_RIGHT_].loadFromFile("img/right.png");
	gDirectTexture[SDL_SCANCODE_LEFT_].loadFromFile("img/left.png");
	gDirectTexture[SDL_SCANCODE_DEFAULT_].loadFromFile("img/press.png");
	return success;
}

void close() {

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
	if (loadedSurface == NULL)
	{
		cout << "unable read the image";
	}
	else {
		SDL_SetColorKey(loadedSurface, SDL_TRUE,
			SDL_MapRGB(loadedSurface->format, COLOR_KEY_R_, COLOR_KEY_G_, COLOR_KEY_B_));
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