#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <string>
#include <vector>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
struct Circle
{
	int x, y;
	int r;
};


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

	static const int DOT_VEL = 3;

	Dot();
	Dot(int x, int y);
	void handleEvent(SDL_Event& e);
	void move(SDL_Rect& square, Circle& circle);
	void render();
	void shiftColliders();
	Circle& getCollider();


private:
	int posX, posY;
	int velX, velY;
	Circle collider;
	vector<SDL_Rect> colliders;

};

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

bool init();
SDL_Texture* loadTexture(string path);
bool loadMedia();
bool checkCollision(Circle& a, Circle& b);
bool checkCollision(Circle& a, SDL_Rect& b);
double distanceSquared(int x1, int y1, int x2, int y2);
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
		Dot dot(Dot::DOT_WIDTH / 2, Dot::DOT_HEIGHT / 2);
		Dot otherDot(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);
		SDL_Rect wall;
		wall.x = 300;
		wall.y = 40;
		wall.w = 40;
		wall.h = 400;
		while (quit == false) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				dot.handleEvent(e);
			}
			dot.move(wall, otherDot.getCollider());

			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderClear(gRenderer);

			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(gRenderer, &wall);

			dot.render();
			otherDot.render();
			SDL_RenderPresent(gRenderer);

			SDL_Delay(25);		}
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
	if (!gTexture.loadFromFile("hello world", textColor))
	{
		cout << "failed to render text texture";
		success = false;
	}

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
	SDL_Surface* textSurface = SDL_LoadBMP("img/dot.bmp");
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

void Dot::render()
{
	gTexture.render(posX - collider.r, posY - collider.r);
}


bool checkCollision(SDL_Rect a, SDL_Rect b)
{
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = a.x;
	leftB = b.x;
	rightA = a.x + a.w;
	rightB = b.x + b.w;
	topA = a.y;
	topB = b.y;
	bottomA = a.y + a.h;
	bottomB = b.y + b.h;

	if (bottomA <= topB) return false;
	if (topA >= bottomB) return false;
	if (rightA <= leftB) return false;
	if (leftA >= rightB) return false;

	return true;

}

Dot::Dot(int x, int y)
{
	posX = x;
	posY = y;

	collider.r = DOT_WIDTH / 2;

	velX = 0;
	velY = 0;
	shiftColliders();
}

void Dot::shiftColliders()
{
	collider.x = posX;
	collider.y = posY;
}

bool checkCollision(vector<SDL_Rect>& a, vector<SDL_Rect>& b)
{
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	for (int abox = 0; abox < a.size(); abox++)
	{
		leftA = a[abox].x;
		rightA = a[abox].x + a[abox].w;
		topA = a[abox].y;
		bottomA = a[abox].y + a[abox].h;
		for (int bbox = 0; bbox < b.size(); bbox++)
		{
			leftB = b[bbox].x;
			rightB = b[bbox].x + b[bbox].w;
			topB = b[bbox].y;
			bottomB = b[bbox].y + b[bbox].h;

			if (((bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB)) == false)
			{
				return true;
			}
		}
	}

	return false;
}

void Dot::move(SDL_Rect& square, Circle& circle)
{
	posX += velX;
	shiftColliders();
	if ((posX - collider.r < 0) || (posX + collider.r > SCREEN_WIDTH) || checkCollision(collider, square) || checkCollision(collider, circle))
	{
		posX -= velX;
		shiftColliders();
	}
	posY += velY;
	shiftColliders();

	if ((posY - collider.r < 0) || (posY + collider.r > SCREEN_HEIGHT) || checkCollision(collider, square) || checkCollision(collider, circle))
	{
		posY -= velY;
		shiftColliders();
	}
}

Circle& Dot::getCollider()
{
	// TODO: insert return statement here
	return collider;
}


bool checkCollision(Circle& a, Circle& b)
{
	int totalRadiusSquared = a.r + b.r;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

	if (distanceSquared(a.x, a.y, b.x, b.y) < totalRadiusSquared)
	{
		return true;
	}
	return false;
}

bool checkCollision(Circle& a, SDL_Rect& b)
{
	int cX, cY;
	if (a.x < b.x)
	{
		cX = b.x;
	}
	else if (a.x > b.x + b.w)
	{
		cX = b.x + b.w;
	}
	else
	{
		cX = a.x;
	}
	if (a.y < b.y)
	{
		cY = b.y;
	}
	else if (a.y > b.y + b.h)
	{
		cY = b.y + b.h;
	}
	else
	{
		cY = a.y;
	}

	if (distanceSquared(a.x, a.y, cX, cY) < a.r * a.r)
	{
		return true;
	}
	return false;
}

double distanceSquared(int x1, int y1, int x2, int y2)
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;


	return deltaX * deltaX + deltaY * deltaY;
}