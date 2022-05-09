#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <string>
#include <sstream>
#include <SDL_ttf.h>
using namespace std;

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;
const int TOTAL_PARTICLES = 20;

class Texture
{
public:
	Texture();
	~Texture();

	void free();
	bool loadFromRenderText(string path);
	void render(int x, int y, SDL_Rect* clip = NULL);

	int getWidth() const { return width; }
	int getHeight() const { return height; }
private:
	SDL_Texture* g_texture;
	int width;
	int height;
};

Texture gDotTexture;
Texture gRedTexture;
Texture gGreenTexture;
Texture gBlueTexture;
Texture gShrimmerTexture;

class Particle
{
public:
	Particle(int x, int y);
	~Particle();
	void render();
	bool isDead();

private:
	int posX, posY;
	int frame;
	Texture* texture_;
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
	void renderParticles();

	int getPosX() const { return posX; }
	int getPosY() const { return posY; }
private:
	int posX, posY;
	int velX, velY;
	Particle* particles[TOTAL_PARTICLES];
};


Dot::Dot()
{
	posX = 0;
	posY = 0;

	velX = 0;
	velY = 0;
	for (int i = 0; i < TOTAL_PARTICLES; i++)
	{
		particles[i] = new Particle(posX, posY);
	}
}

void Dot::renderParticles()
{
	for (int i = 0; i < TOTAL_PARTICLES; i++)
	{
		if (particles[i]->isDead())
		{
			delete particles[i];
			particles[i] = new Particle(posX, posY);
		}
	}
	for (int i = 0; i < TOTAL_PARTICLES; i++)
	{
		particles[i]->render();
	}
}
Particle::Particle(int x, int y)
{
	posX = x - 5 + (rand() % 25);
	posY = y - 5 + (rand() % 25);

	frame = rand() % 5;
	switch (rand() % 3)
	{
	case 0: texture_ = &gRedTexture; break;
	case 1: texture_ = &gGreenTexture; break;
	case 2: texture_ = &gBlueTexture; break;
	default:
		break;
	}
}

Particle::~Particle()
{
}

void Particle::render()
{
	texture_->render(posX, posY);
	if (frame % 2 == 0)
	{
		gShrimmerTexture.render(posX, posY);
	}
	frame++;
}

bool Particle::isDead()
{
	return frame > 10;
}

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;

bool init();
SDL_Texture* loadTexture(string path);
bool loadMedia();
void close();

Texture gTexture;

int main(int argc, char* args[]) {
	init();

	if (loadMedia() == false) {
		return 1;
	}
	else {
		bool quit = false;
		SDL_Event e;
		Dot dot;

		while (!quit) {
			bool renderText = false;
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				dot.handleEvent(e);
			}
			dot.move();
			
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderClear(gRenderer);

			dot.render();
			SDL_RenderPresent(gRenderer);
			SDL_Delay(100);
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
	gDotTexture.loadFromRenderText("img/dot.bmp");
	gRedTexture.loadFromRenderText("img/red.bmp");
	gGreenTexture.loadFromRenderText("img/green.bmp");
	gBlueTexture.loadFromRenderText("img/blue.bmp");
	gShrimmerTexture.loadFromRenderText("img/shimmer.bmp");
	
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

bool Texture::loadFromRenderText(string path)
{
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		cout << "unable read the image";
	}
	else {
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
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
	renderParticles();
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