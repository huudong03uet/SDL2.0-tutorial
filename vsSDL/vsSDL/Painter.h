#ifndef PAINTER_H_
#define PAINTER_H_
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
const SDL_Color CYAN_COLOR = { 0, 255, 255 };
const SDL_Color BLUE_COLOR = { 0, 0, 255 };
const SDL_Color ORANGE_COLOR = { 255, 165, 0 };
const SDL_Color YELLOW_COLOR = { 255, 255, 0 };
const SDL_Color LIME_COLOR = { 0, 255, 0 };
const SDL_Color PURPLE_COLOR = { 128, 0, 128 };
const SDL_Color RED_COLOR = { 255, 0, 0 };
const SDL_Color WHITE_COLOR = { 255, 255, 255 };
const SDL_Color BLACK_COLOR = { 0, 0, 0 };
const SDL_Color GREEN_COLOR = { 0, 128, 0 };

const SDL_Color DEFAULT_COLOR = WHITE_COLOR;

class Painter
{
public:
	Painter();
	Painter(SDL_Window* gWindow, SDL_Renderer* gRenderer);
	void setPosition(double x, double y);

	double getX() const { return x; }
	double getY() const { return y; }

	void setAngle(double angle);
	double getAngle() const { return angle; }

	void setColor(SDL_Color color);
	SDL_Color getColor() const { return color; }

	SDL_Renderer* getRenderer() const { return renderer; }

	void clearWithBackGroundColor(SDL_Color color);
	void moveForward(double length);
	void jumpForward(double length);
	void moveBackward(double length) { moveForward(-length); }
	void jumpBackward(double length) { jumpForward(-length); }
	void turnLeft(double angle) { setAngle(this->angle + angle); }
	void turnRight(double angle) { turnLeft(-angle); }

	void setRandomColor();
private:
	double x;
	double y;
	double angle;

	SDL_Color color;
	SDL_Renderer* renderer;
};
#endif // !PAINTER_H_

