#include "Painter.h"

Painter::Painter()
{

}

Painter::Painter(SDL_Window* gWindow, SDL_Renderer* gRenderer_)
	:renderer(gRenderer_)
{
	int width, height;
	SDL_RenderGetLogicalSize(renderer, &width, &height);

	if (width == 0 && height == 0)
		SDL_GetWindowSize(gWindow, &width, &height);

	setPosition(width / 2, height / 2);
	setAngle(0);
	setColor(BLACK_COLOR);
	clearWithBackGroundColor(BLUE_COLOR);
}

void Painter::setPosition(double x, double y)
{
	this->x = x;
	this->y = y;
}

void Painter::setAngle(double angle)
{
	this->angle = angle - 360 * floor(angle / 360);
}

void Painter::setColor(SDL_Color color)
{
	this->color = color;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0);
}

void Painter::clearWithBackGroundColor(SDL_Color backgroundColor)
{
	SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0);
}

void Painter::moveForward(double length)
{
	double prevX = x, prevY = y;
	jumpForward(length);
	SDL_RenderDrawLine(renderer, (int)prevX, (int)prevY, (int)x, (int)y);

}

void Painter::jumpForward(double length)
{
	double rad = (angle / 180) * M_PI;
	x += cos(rad) * length;
	y -= sin(rad) * length;
}

void Painter::setRandomColor()
{
	Uint8 r = rand() % 256;
	Uint8 g = rand() % 256;
	Uint8 b = rand() % 256;
	SDL_Color color = { r, g, b };
	setColor(color);
}