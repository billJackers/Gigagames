#ifndef SNAKE_NODE_H
#define SNAKE_NODE_H

#include "coordinate.h"

enum Direction
{
    UP,
    LEFT,
    DOWN,
    RIGHT
};

class SnakeNode
{
public:
    Coordinate position;
    SnakeNode* next;
    Direction direction;
    Direction prevDirection;

    SnakeNode(int xPos, int yPos, SnakeNode* _next, Direction _direction)
        : next(_next), direction(_direction)
    {
        position = Coordinate(xPos, yPos);
        prevDirection = direction;
    };

    ~SnakeNode()
    {
        
    }
};

#endif