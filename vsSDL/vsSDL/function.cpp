#include "function.h"

double generateRamdomNumber()
{
	return (double)rand() / RAND_MAX;
}

void randomWalk(Painter& painter)
{
	painter.setRandomColor();
	for (int i = 0; i < STEPS_WALK; i++)
	{
		double angle = generateRamdomNumber() * 360;
		painter.turnRight(angle);

		double length = generateRamdomNumber() * STEPS_WALK;
		painter.moveForward(length);
	}
}

void drawSquare(Painter& painter, double size)
{
	for (int i = 0; i < 4; i++)
	{
		//painter.setRandomColor();
		painter.moveForward(size);
		painter.turnRight(90);
	}
}


void fillSquare(Painter& painter)
{
	for (int i = 0; i < SIZE; i += 1)
	{
		drawSquare(painter, i);
		painter.jumpBackward(1);
	}
}

void drawTriangle(Painter& painter, double size)
{
	for (int i = 0; i < 3; i++)
	{
		painter.turnLeft(120);
		painter.moveForward(size);
	}
}

void fillTriangle(Painter& painter)
{
	for (int i = SIZE; i >=0 ; i -= 1)
	{
		drawTriangle(painter, i);
		painter.jumpBackward(0.5);
	}
}

void drawOctagon(Painter& painter, double size)
{
	painter.setPosition(START_X_, START_Y_);
	{
		for (int i = 0; i < 8; i++)
		{
			painter.moveForward(size);
			painter.turnLeft(45);
		}
	}
}

void fillOctagon(Painter& painter)
{
	for (double i = 0; i < SIZE; i += 0.25)
	{
		drawOctagon(painter, i);
	}
}

void drawStarOfFives(Painter& painter, double size)
{
	for (int i = 0; i < 5; i++)
	{
		painter.moveForward(size);
		painter.turnRight(144);
	}
}

void fillStarOfFives(Painter& painter)
{
	for (double i = 0; i <= SIZE; i ++)
	{
		painter.moveBackward(0.5);
		drawStarOfFives(painter, i);
	}
}

void drawStarOfDavid(Painter& painter, double size)
{
	//painter.setPosition(START_X_, START_Y_);
	drawTriangle(painter, size);
	painter.turnRight(30);
	painter.jumpBackward(size * 2 / sqrt(3));
	painter.turnRight(150);
	drawTriangle(painter, size);
}

void fillStarOfDavid(Painter& painter)
{
	double prevX = painter.getX(), prevY = painter.getY();
	double prevAngle = 0;
	for (double i = 0; i < SIZE ; i++)
	{
		painter.setPosition(prevX, prevY);
		painter.setAngle(prevAngle);
		painter.jumpForward(i / 2);

		drawStarOfDavid(painter, i);
		
	}
}

void drawEightLinesCrossingAtCenter(Painter& painter)
{
	for (int i = 0; i < 8; i++)
	{
		painter.moveForward(SIZE);
		painter.jumpBackward(SIZE);
		painter.turnLeft(45);
	}
}

void drawSixSquares(Painter& painter, double size)
{
	for (int i = 0; i < 6; i++)
	{
		drawSquare(painter, size);
		painter.turnRight(60);
	}
}

void fillSixSquares(Painter& painter)
{
	for (double i = 0; i < SIZE; i += 0.5)
	{
		drawSixSquares(painter, i);
	}
}

void drawCircle(Painter& painter, SDL_Renderer* gRenderer, double radius)
{
	double rad = (painter.getAngle() / 180) * M_PI;
	int centerX = painter.getX() + cos(rad) * radius;
	int centerY = painter.getY() - sin(rad) * radius;
	int dx = radius;
	int dy = 0;
	int err = 0;
	while (dx > dy)
	{
		SDL_RenderDrawPoint(gRenderer, centerX + dx, centerY + dy);
		SDL_RenderDrawPoint(gRenderer, centerX + dy, centerY + dx);
		SDL_RenderDrawPoint(gRenderer, centerX - dy, centerY + dx);
		SDL_RenderDrawPoint(gRenderer, centerX - dx, centerY + dy);
		SDL_RenderDrawPoint(gRenderer, centerX - dx, centerY - dy);
		SDL_RenderDrawPoint(gRenderer, centerX - dy, centerY - dx);
		SDL_RenderDrawPoint(gRenderer, centerX + dy, centerY - dx);
		SDL_RenderDrawPoint(gRenderer, centerX + dx, centerY - dy);
		
		if (err <= 0) {
			dy += 1;
			err += 2 * dy + 1;
		}

		if (err > 0) {
			dx -= 1;
			err -= 2 * dx + 1;
		}
	}

}

void fillCircle(Painter& painter, SDL_Renderer* gRenderer)
{
	for (double i = 0; i <= SIZE; i ++)
	{
		painter.jumpBackward(1);
		drawCircle(painter, gRenderer, i);
	}
}

void drawNumberSquare(Painter& painter, int number, double size)
{
	for (int i = 0; i < number; i++)
	{
		drawSquare(painter, size);
		painter.turnRight(360.0  / number);
	}
}

void drawMultitudeLinesInCicles(Painter& painter, int number, double size)
{
	for (int i = 0; i < number; i++)
	{
		painter.setRandomColor();
		painter.jumpForward(size);
		painter.moveBackward(size);
		painter.turnRight(360.0 / number);
	}
}

void drawParallelogram(Painter& painter, double size)
{
	painter.turnRight(30);
	for (int i = 0; i < 2; i++)
	{
		painter.moveForward(size);
		painter.turnLeft(60);
		painter.moveForward(size);
		painter.turnLeft(120);
	}
}

void fillParallelogram(Painter& painter)
{
	for (int i = 0; i <= SIZE; i ++)
	{
		painter.moveBackward(i * sqrt(3) / 2 - (i - 1) * sqrt(3) / 2);
		drawParallelogram(painter, i);
		painter.turnLeft(30);
	}
}

void drawNumberParallelogram(Painter& painter, int number, double size)
{
	for (int i = 0; i < number; i++)
	{
		drawParallelogram(painter, size);
		painter.turnRight(360.0 / number - 30);
	}
}

void drawFiveAndFiveCirles(Painter& painter, SDL_Renderer* gRenderer)
{
	for (int i = 0; i < 5; i++)
	{
		drawCircle(painter, gRenderer, 100);
		drawCircle(painter, gRenderer, 50);
		painter.turnRight(72);
	}
}

void drawSnowFlake(Painter& painter)
{
	int size = 20;
	for (int i = 0; i < 8; i++)
	{
		painter.moveForward(size);
		for (int j = 0; j < 3; j++)
		{
			painter.turnLeft(45);
			painter.moveForward(size);
			painter.jumpBackward(size);
			painter.turnRight(90);
			painter.moveForward(size);
			painter.jumpBackward(size);
			painter.turnLeft(45);
			painter.moveForward(size);
		}
		painter.jumpBackward(4 * size);
		painter.turnLeft(45);
	}
}
