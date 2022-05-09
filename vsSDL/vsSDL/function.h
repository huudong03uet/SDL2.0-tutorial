#ifndef FUNCTION_H_
#define FUNCTION_H_
#include "Painter.h"

const int STEPS_WALK = 100;
const int SIZE = 100;

#define START_X_ 400
#define START_Y_ 400
double generateRamdomNumber();
void randomWalk(Painter& painter);

void drawSquare(Painter& painter, double size = SIZE);
void fillSquare(Painter& painter);

void drawTriangle(Painter& painter, double size = SIZE);
void fillTriangle(Painter& painter);

void drawOctagon(Painter& painter, double size = SIZE);
void fillOctagon(Painter& painter);

void drawStarOfFives(Painter& painter, double size = SIZE);
void fillStarOfFives(Painter& painter);

void drawStarOfDavid(Painter& painter, double size = SIZE);
void fillStarOfDavid(Painter& painter);

void drawEightLinesCrossingAtCenter(Painter& painter);

void drawSixSquares(Painter& painter, double size = SIZE);
void fillSixSquares(Painter& painter);

void drawCircle(Painter& painter, SDL_Renderer* gRenderer, double radius = SIZE);
void fillCircle(Painter& painter, SDL_Renderer* gRenderer);

void drawNumberSquare(Painter& painter, int number = 6, double size = SIZE);

void drawMultitudeLinesInCicles(Painter& painter, int number = 8, double size= SIZE);

void drawParallelogram(Painter& painter, double size = SIZE);
void fillParallelogram(Painter& painter);
void drawNumberParallelogram(Painter& painter, int number, double size = SIZE);

void drawFiveAndFiveCirles(Painter& painter, SDL_Renderer* gRenderer);

void drawSnowFlake(Painter& painter);
#endif // !FUNCTION_H_

