#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <string>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

enum BUTTON_SPRITE
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
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

Texture gAnimationTexture;

SDL_Rect gRectAnimationClips[4];

class Button
{
public:
	Button();
	void setPosition(int x, int y);
	void handleEvent(SDL_Event* e);
	void render();

private:
	SDL_Point g_position_; 
	BUTTON_SPRITE g_current_sprite_;
};

Button gButton[TOTAL_BUTTONS];

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

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
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderClear(gRenderer);
		while (quit == false) {
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				for (int i = 0; i < TOTAL_BUTTONS; i++)
				{
					gButton[i].handleEvent(&e);
				}
			}
			for (int i = 0; i < TOTAL_BUTTONS; i++)
			{
				gButton[i].render();
				SDL_RenderPresent(gRenderer);
			}

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
	if (!gAnimationTexture.loadFromFile("img/button.png"))
	{
		cout << "failed to load";
		success = false;
	}

	for (int i = 0; i < BUTTON_SPRITE_TOTAL; i++)
	{
		gRectAnimationClips[i].x = 0;
		gRectAnimationClips[i].y = i * 200;
		gRectAnimationClips[i].w = BUTTON_WIDTH;
		gRectAnimationClips[i].h = BUTTON_HEIGHT;
	}

	gButton[0].setPosition(0, 0);
	gButton[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 0);
	gButton[2].setPosition(0, SCREEN_HEIGHT - BUTTON_HEIGHT);
	gButton[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
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

Button::Button()
{
	g_position_.x = 0;
	g_position_.y = 0;
	g_current_sprite_ = BUTTON_SPRITE_MOUSE_OUT;
}

void Button::setPosition(int x, int y)
{
	g_position_.x = x;
	g_position_.y = y;
}

void Button::handleEvent(SDL_Event* e)
{
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		bool inside = true;
		if (x < g_position_.x)
		{
			inside = false;
		}
		else if (x > g_position_.x + BUTTON_WIDTH)
		{
			inside = false;
		}
		else if (y < g_position_.y)
		{
			inside = false;
		}
		else if (y > g_position_.y + BUTTON_HEIGHT)
		{
			inside = false;
		}

		if (!inside)
		{
			g_current_sprite_ = BUTTON_SPRITE_MOUSE_OUT;
		}
		else
		{
			switch (e->type)
			{
			case SDL_MOUSEMOTION: g_current_sprite_ = BUTTON_SPRITE_MOUSE_OVER_MOTION; break;
			case SDL_MOUSEBUTTONDOWN: g_current_sprite_ = BUTTON_SPRITE_MOUSE_DOWN; break;
			case SDL_MOUSEBUTTONUP: g_current_sprite_ = BUTTON_SPRITE_MOUSE_UP; break;
			}
		}
	}
}

void Button::render()
{
	gAnimationTexture.render(g_position_.x, g_position_.y, &gRectAnimationClips[g_current_sprite_]);
}