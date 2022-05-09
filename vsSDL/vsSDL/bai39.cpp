#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The dimensions of the level
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

//Tile constants
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 12;

//The different tile sprites
const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;

class Tile
{
public:
	Tile();
	~Tile();
	Tile(int x, int y, int tileType);
	void render(SDL_Rect& camera);
	int getType();
	SDL_Rect getBox();
private:

	SDL_Rect box_;
	int type_;
};

SDL_Rect gTileClips[TOTAL_TILE_SPRITES];
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
Texture gTileTexture;
Texture gDotTexture;
class Dot
{
public:
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;

	static const int DOT_VEL = 10;
	Dot();
	void handleEvent(SDL_Event& e);
	void move(Tile* tiles[]);
	void render(SDL_Rect& camera);
	void setCamera(SDL_Rect& camera);
	int getPosX() const { return posX; }
	int getPosY() const { return posY; }
private:
	int posX, posY;
	int velX, velY;
	SDL_Rect box_;
};
Dot dot;

SDL_Renderer* gRenderer;
bool init();
bool loadMedia(Tile* tiles[]);
void close(Tile* tiles[]);
bool checkCollision(SDL_Rect a, SDL_Rect b);
bool touchesWall(SDL_Rect box, Tile* tiles[]);
bool setTiles(Tile* tiles[]);
SDL_Window* gWindow;
Tile::Tile()
{

}
Tile::Tile(int x, int y, int tileType)
{
	box_.x = x;
	box_.y = y;
	box_.w = TILE_WIDTH;
	box_.h = TILE_HEIGHT;
	type_ = tileType;
}

Tile* tileSet[TOTAL_TILES];

void Tile::render(SDL_Rect& camera)
{
	if (checkCollision(camera, box_))
	{
		gTileTexture.render(box_.x - camera.x, box_.y - camera.y, &gTileClips[type_]);
	}
}

int Tile::getType()
{
	return type_;
}

SDL_Rect Tile::getBox()
{
	return box_;
}

void Dot::move(Tile* tiles[])
{
	box_.x += velX;
	if ((box_.x < 0) || (box_.x + DOT_WIDTH > LEVEL_WIDTH) || touchesWall(box_, tiles))
	{
		box_.x -= velX;
	}
	box_.y += velY;
	if ((box_.y < 0) || (box_.y + DOT_HEIGHT > LEVEL_HEIGHT) || touchesWall(box_, tiles))
	{
		box_.y -= velY;
	}
}

void Dot::render(SDL_Rect& camera)
{
	gDotTexture.render(box_.x - camera.x, box_.y - camera.y);
}

void Dot::setCamera(SDL_Rect& camera)
{
	camera.x = (box_.x + DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (box_.y + DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;
	if (camera.x < 0)
	{
		camera.x = 0;
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.x > LEVEL_WIDTH - camera.w)
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h)
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

bool loadMedia(Tile* tiles[])
{
	bool success = true;
	gDotTexture.loadFromFile("img/dot.bmp");
	gTileTexture.loadFromFile("img/tiles.png");
	setTiles(tiles);
	return success;
}

bool setTiles(Tile* tiles[])
{
	bool tilesLoaded = true;
	int x = 0, y = 0;
	ifstream map("map/lazy.map");
	if (map.fail())
	{
		cout << "unable to load map file";
		tilesLoaded = false;
	} 
	else
	{
		for (int i = 0; i < TOTAL_TILES; i++)
		{
			int tileType = -1;
			map >> tileType;
			if (map.fail())
			{
				cout << "Error  loading map:";
				tilesLoaded = false;
				break;
			}

			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				tiles[i] = new Tile(x, y, tileType);

			}
			else {
				cout << "error loading map:";
				tilesLoaded = false; break;
			}
			x += TILE_WIDTH;
			if (x >= LEVEL_WIDTH)
			{
				x = 0;
				y += TILE_HEIGHT;
			}
		}
	}
	if (tilesLoaded) {
		gTileClips[TILE_RED].x = 0;
		gTileClips[TILE_RED].y = 0;
		gTileClips[TILE_RED].w = TILE_WIDTH;
		gTileClips[TILE_RED].h = TILE_HEIGHT;

		gTileClips[TILE_GREEN].x = 0;
		gTileClips[TILE_GREEN].y = 80;
		gTileClips[TILE_GREEN].w = TILE_WIDTH;
		gTileClips[TILE_GREEN].h = TILE_HEIGHT;

		gTileClips[TILE_BLUE].x = 0;
		gTileClips[TILE_BLUE].y = 160;
		gTileClips[TILE_BLUE].w = TILE_WIDTH;
		gTileClips[TILE_BLUE].h = TILE_HEIGHT;

		gTileClips[TILE_TOPLEFT].x = 80;
		gTileClips[TILE_TOPLEFT].y = 0;
		gTileClips[TILE_TOPLEFT].w = TILE_WIDTH;
		gTileClips[TILE_TOPLEFT].h = TILE_HEIGHT;

		gTileClips[TILE_LEFT].x = 80;
		gTileClips[TILE_LEFT].y = 80;
		gTileClips[TILE_LEFT].w = TILE_WIDTH;
		gTileClips[TILE_LEFT].h = TILE_HEIGHT;

		gTileClips[TILE_BOTTOMLEFT].x = 80;
		gTileClips[TILE_BOTTOMLEFT].y = 160;
		gTileClips[TILE_BOTTOMLEFT].w = TILE_WIDTH;
		gTileClips[TILE_BOTTOMLEFT].h = TILE_HEIGHT;

		gTileClips[TILE_TOP].x = 160;
		gTileClips[TILE_TOP].y = 0;
		gTileClips[TILE_TOP].w = TILE_WIDTH;
		gTileClips[TILE_TOP].h = TILE_HEIGHT;

		gTileClips[TILE_CENTER].x = 160;
		gTileClips[TILE_CENTER].y = 80;
		gTileClips[TILE_CENTER].w = TILE_WIDTH;
		gTileClips[TILE_CENTER].h = TILE_HEIGHT;

		gTileClips[TILE_BOTTOM].x = 160;
		gTileClips[TILE_BOTTOM].y = 160;
		gTileClips[TILE_BOTTOM].w = TILE_WIDTH;
		gTileClips[TILE_BOTTOM].h = TILE_HEIGHT;

		gTileClips[TILE_TOPRIGHT].x = 240;
		gTileClips[TILE_TOPRIGHT].y = 0;
		gTileClips[TILE_TOPRIGHT].w = TILE_WIDTH;
		gTileClips[TILE_TOPRIGHT].h = TILE_HEIGHT;

		gTileClips[TILE_RIGHT].x = 240;
		gTileClips[TILE_RIGHT].y = 80;
		gTileClips[TILE_RIGHT].w = TILE_WIDTH;
		gTileClips[TILE_RIGHT].h = TILE_HEIGHT;

		gTileClips[TILE_BOTTOMRIGHT].x = 240;
		gTileClips[TILE_BOTTOMRIGHT].y = 160;
		gTileClips[TILE_BOTTOMRIGHT].w = TILE_WIDTH;
		gTileClips[TILE_BOTTOMRIGHT].h = TILE_HEIGHT;
	}
	map.close();
	return tilesLoaded;
}

bool touchesWall(SDL_Rect box_, Tile* tiles[])
{
	for (int i = 0; i < TOTAL_TILES; i++)
	{
		if ((tiles[i]->getType() >= TILE_CENTER) && (tiles[i]->getType() <= TILE_TOPLEFT))
		{
			if (checkCollision(box_, tiles[i]->getBox()))
			{
				return true;
			}
		}
	}
	return false;
}

int main(int argc, char* argv[])
{
	init();
	loadMedia(tileSet);
	bool endGame = false;
	SDL_Event e;
	SDL_Rect camera = { 0,0, SCREEN_WIDTH, SCREEN_HEIGHT };
	while (!endGame)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				endGame = true;
			}
			dot.handleEvent(e);
			
		}
		dot.move(tileSet);
		dot.setCamera(camera);

		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderClear(gRenderer);

		for (int i = 0; i < TOTAL_TILES; i++)
		{
			tileSet[i]->render(camera);
		}
		dot.render(camera);
		SDL_RenderPresent(gRenderer);

	}
	return 0;
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
	SDL_Surface* textSurface = IMG_Load(path.c_str());
	if (textSurface == NULL)
	{
		cout << "unable read the image";
	}
	else {

		SDL_SetColorKey(textSurface, SDL_TRUE, SDL_MapRGB(textSurface->format, 0, 0xFF, 0xFF));
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
	box_.w = DOT_WIDTH;
	box_.h = DOT_HEIGHT;
	velX = 0;
	velY = 0;
}

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}