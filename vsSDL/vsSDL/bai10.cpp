#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <string>
using namespace std;

#define COLER_KEY_R_ 167
#define COLOR_KEY_G_ 175
#define COLOR_KEY_B_ 180

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;

class Texture
{
public:
	Texture();
	~Texture();

	void free();
	bool loadFromFile(string path);
	void render(int x, int y);

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
Texture gBackground;
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
		while (quit == false) {
			if (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
			}
			gBackground.render(0, 0);
			gTexture.render(240, 190);
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
	if (!gTexture.loadFromFile("img/boss_bullet.png"))
	{
		cout << "failed to load";
		success = false;
	}
	if (!gBackground.loadFromFile("img/background.png")) {
		cout << "failed to load background";
		success = false;
	}
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

void Texture::render(int x, int y)
{
	SDL_Rect renderQuad = { x, y, width, height };
	SDL_RenderCopy(gRenderer, g_texture, NULL, &renderQuad);
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
			SDL_MapRGB(loadedSurface->format, COLER_KEY_R_, COLOR_KEY_G_, COLOR_KEY_B_));
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