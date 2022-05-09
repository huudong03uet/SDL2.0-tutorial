#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <string>
#include <sstream>
#include <SDL_ttf.h>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TOTAL_DATA = 10;

class Texture
{
public:
	Texture();
	~Texture();

	void free();
	bool loadFromRenderText(string path, SDL_Color textColor);
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
TTF_Font* gFont = NULL;

bool init();
SDL_Texture* loadTexture(string path);
bool loadMedia();
void close();

Texture gTexture;
Texture gDataTexture[TOTAL_DATA];
Sint32 gData[TOTAL_DATA];
int main(int argc, char* args[]) {
	init();

	if (loadMedia() == false) {
		return 1;
	}
	else {
		bool quit = false;
		SDL_Event e;

		SDL_Color textColor = { 0,0,0, 255 };
		SDL_Color hightLightColor = { 0,0,255, 255 };
		int currentData = 0;
		while (!quit) {
			bool renderText = false;
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				else if (e.type == SDL_KEYDOWN)
				{
					switch (e.key.keysym.sym)
					{
					case SDLK_UP:
						gDataTexture[currentData].loadFromRenderText(to_string(gData[currentData]), textColor);
						--currentData;
						if (currentData < 0)
						{
							currentData = TOTAL_DATA - 1;
						}
						gDataTexture[currentData].loadFromRenderText(to_string(gData[currentData]), hightLightColor);
						break;

					case SDLK_DOWN:
						gDataTexture[currentData].loadFromRenderText(to_string(gData[currentData]), textColor);
						currentData++;
						if (currentData == TOTAL_DATA)
						{
							currentData = 0;
						}
						gDataTexture[currentData].loadFromRenderText(to_string(gData[currentData]), hightLightColor);
						break;
					case SDLK_LEFT:
						--gData[currentData];
						gDataTexture[currentData].loadFromRenderText(to_string(gData[currentData]), hightLightColor);
						break;

					case SDLK_RIGHT:
						++gData[currentData];
						gDataTexture[currentData].loadFromRenderText(to_string(gData[currentData]), hightLightColor);
						break;
					default:
						break;
					}
				}
			}

			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderClear(gRenderer);

			gTexture.render((SCREEN_WIDTH - gTexture.getWidth()) / 2, 0);
			for (int i = 0; i < TOTAL_DATA; i++)
			{
				gDataTexture[i].render((SCREEN_WIDTH - gDataTexture[i].getWidth()) / 2, gTexture.getHeight() + gDataTexture[0].getHeight() * i);

			}

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

bool loadMedia() {
	bool success = true;
	gFont = TTF_OpenFont("font/font.ttf", 15);
	if (gFont == NULL)
	{
		cout << "failed to load lazy font";
	}
	SDL_RWops* file = SDL_RWFromFile("file/nums.bin", "r+b");
	if (file == NULL) {
		cout << "error file";
		file = SDL_RWFromFile("file/nums.bin", "w+b");
		if (file != NULL) {
			cout << "new file created";
			for (int i = 0; i < TOTAL_DATA; i++)
			{
				gData[i] = 0;
				SDL_RWwrite(file, &gData[i], sizeof(Sint32), 1);
			}
			SDL_RWclose(file);
		}
		else {
			cout << "unable to create file";
			success = false;
		}
	}
	else
	{
		cout << "reading file";
		for (int i = 0; i < TOTAL_DATA; i++)
		{
			SDL_RWread(file, &gData[i], sizeof(Sint32), 1);

		}
		SDL_RWclose(file);
	}
	SDL_Color highlightColor = { 0,0,0, 255 };
	gDataTexture[0].loadFromRenderText(to_string(gData[0]), highlightColor);
	for (int i = 1; i < TOTAL_DATA; i++)
	{
		gDataTexture[i].loadFromRenderText(to_string(gData[i]), highlightColor);
	}
	return success;
}

void close() {
	SDL_RWops* file = SDL_RWFromFile("file/nums.bin", "w+b");
	if (file != NULL)
	{
		for (int i = 0; i < TOTAL_DATA; i++)
		{
			for (int i = 0; i < TOTAL_DATA; i++)
			{
				SDL_RWwrite(file, &gData[i], sizeof(Sint32), 1);

			}
			SDL_RWclose(file);
		}
	}
	else {
		cout << "unable to save file";

	}
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

bool Texture::loadFromRenderText(string path, SDL_Color textColor)
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