#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <string>
#include <sstream>
#include <SDL_ttf.h>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Window
{
public:
	Window();
	~Window();
	bool init();
	SDL_Renderer* createRenderer();
	void handleEvent(SDL_Event& e);
	void free();

	int getWidth();
	int getHeight();

	bool hasMouseFocus();
	bool hasKeyBoardFocus();
	bool isMinimized();
private:

	SDL_Window* window_;
	int width_;
	int height_;

	bool mouseFocus;
	bool keyboardFocus;
	bool fullScreen;
	bool minimized;
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

Window gWindow;
SDL_Renderer* gRenderer = NULL;
Texture gSceneTexture;

int main(int argc, char* argv[])
{
	if (!gWindow.init())
	{
		cout << "Window could not be created!";
		return 0;
	}
	else {
		gRenderer = gWindow.createRenderer();
		if (gRenderer == NULL)
		{
			cout << "Renderer could not be created";
			return 0;
		}
	}
	gSceneTexture.loadFromFile("img/bg.png");
	bool quit = false;
	SDL_Event e;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			gWindow.handleEvent(e);
		}
		if (!gWindow.isMinimized())
		{
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderClear(gRenderer);
			gSceneTexture.render((gWindow.getWidth() - gSceneTexture.getWidth()) / 2, (gWindow.getHeight() - gSceneTexture.getHeight()) / 2);
			SDL_RenderPresent(gRenderer);
		}
	}
}
Window::Window()
{
	window_ = NULL;
	mouseFocus = false;
	keyboardFocus = false;
	fullScreen = false;
	minimized = false;
	width_ = false;
	height_ = false;
}


Window::~Window()
{
}

bool Window::init()
{
	window_ = SDL_CreateWindow("SDL_Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window_ != NULL)
	{
		mouseFocus = true;
		keyboardFocus = true;
		width_ = SCREEN_WIDTH;
		height_ = SCREEN_HEIGHT;
	}
	return window_ != NULL;
}

SDL_Renderer* Window::createRenderer()
{
	return SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void Window::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_WINDOWEVENT)
	{
		bool updateCaption = false;
		switch (e.window.event)
		{
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			width_ = e.window.data1;
			height_ = e.window.data2;
			SDL_RenderPresent(gRenderer);
			break;

		case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent(gRenderer);
			break;

		case SDL_WINDOWEVENT_ENTER:
			mouseFocus = true;
			updateCaption = true;
			break;
			
		case SDL_WINDOWEVENT_LEAVE:
			mouseFocus = false;
			updateCaption = true;
			break;

		case SDL_WINDOWEVENT_FOCUS_GAINED:
			keyboardFocus = true;
			updateCaption = true;
			break;

		case SDL_WINDOWEVENT_FOCUS_LOST:
			keyboardFocus = false;
			updateCaption = true;
			break;

		case SDL_WINDOWEVENT_MINIMIZED:
			minimized = true;
			break;

		case SDL_WINDOWEVENT_MAXIMIZED:
			minimized = false;
			break;

		case SDL_WINDOWEVENT_RESTORED:
			minimized = false;
			break;
		default:
			break;
		}

		if (updateCaption)
		{
			stringstream caption;
			caption << "SDL Tutorial - MouseFocus: " << (mouseFocus ? "On" : "Off") << " KeyboardFocus: " << (keyboardFocus) ? "On" : "Off";
			SDL_SetWindowTitle(window_, caption.str().c_str());
		}
	}
	else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
	{
		if (fullScreen)
		{
			SDL_SetWindowFullscreen(window_, SDL_FALSE);
			fullScreen = false;
		}
		else {
			SDL_SetWindowFullscreen(window_, SDL_TRUE);
			fullScreen = true;
			minimized = false;
		}
	}
}

void Window::free()
{
	gSceneTexture.free();

	SDL_DestroyRenderer(gRenderer);
	gWindow.free();
	IMG_Quit();
	SDL_Quit();
}

int Window::getWidth()
{
	return width_;
}

int Window::getHeight()
{
	return height_;
}

bool Window::hasMouseFocus()
{
	return mouseFocus;
}

bool Window::hasKeyBoardFocus()
{
	return keyboardFocus;
}

bool Window::isMinimized()
{
	return minimized;
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

bool Texture::loadFromFile(string path)
{
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
