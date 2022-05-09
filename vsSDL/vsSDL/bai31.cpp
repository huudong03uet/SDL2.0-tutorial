#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <string>
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

class Dot
{
public:
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;

	static const int DOT_VEL = 10;
	Dot();
	void handleEvent(SDL_Event& e);
	void move();
	void render();

	int getPosX() const { return posX; }
	int getPosY() const { return posY; }
private:
	int posX, posY;
	int velX, velY;

};

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

bool init();
SDL_Texture* loadTexture(string path);
bool loadMedia();
bool checkCollision(SDL_Rect a, SDL_Rect b);
void close();

Texture gTexture;
Texture gDotTexture;

int main(int argc, char* args[]) {
	init();

	if (loadMedia() == false) {
		return 1;
	}
	else {
		bool quit = false;
		SDL_Event e;
		Dot dot;
		int scrollingOffset = 0;
		SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		while (quit == false) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				dot.handleEvent(e);
			}
			dot.move();
			scrollingOffset -= 4;
			
			if (scrollingOffset < -gTexture.getWidth())
			{
				scrollingOffset = 0;
			}

			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderClear(gRenderer);

			gTexture.render(scrollingOffset, 0);
			gTexture.render(scrollingOffset + gTexture.getWidth(), 0);
			dot.render();

			SDL_RenderPresent(gRenderer);
			SDL_Delay(25);
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
	return success;
}

bool loadMedia() {
	bool success = true;


	SDL_Color textColor = { 255, 0, 0 };
	if (!gTexture.loadFromFile("img/background.png", textColor))
	{
		cout << "failed to render text texture";
		success = false;
	}
	gDotTexture.loadFromFile("img/dot.bmp", textColor);

	return success;
}

void close() {

	gTexture.free();

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

bool Texture::loadFromFile(string path, SDL_Color textColor)
{
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* textSurface = IMG_Load(path.c_str());
	if (textSurface == NULL)
	{
		cout << "unable read the image";
	}
	else {

		newTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (newTexture == NULL)
		{
			cout << "unable to create texture from surface";
		}
		else {
			width = textSurface->w;
			height = textSurface->h;
		}

		SDL_FreeSurface(textSurface);
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

void Dot::move()
{
	posX += velX;
	if ((posX < 0) || (posX + DOT_WIDTH > SCREEN_WIDTH))
	{
		posX -= velX;
	}
	posY += velY;
	if ((posY < 0) || (posY + DOT_HEIGHT > SCREEN_HEIGHT))
	{
		posY -= velY;
	}
}

void Dot::render() {
	gDotTexture.render(posX, posY);
}
