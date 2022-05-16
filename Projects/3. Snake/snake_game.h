#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include "coordinate.h"
#include "snake_node.h"
#include "randomizer.h"

class SnakeGame
{
public:
	bool gameOver;
	SnakeNode* head;
	SnakeNode* tail;
	SnakeNode* prevTail;
	Coordinate applePos;
	unsigned int NUM_ROWS;
	unsigned int NUM_COLS;

private:
	Randomizer* xRandomizer;
	Randomizer* yRandomizer;

public:
	SnakeGame()
	{

	};

	SnakeGame()
	{
		xRandomizer = new Randomizer(0, NUM_COLS);
		yRandomizer = new Randomizer(0, NUM_ROWS);
	};
};

#endif