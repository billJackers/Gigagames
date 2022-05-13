#ifndef SNAKE_NODE_H
#define SNAKE_NODE_H

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
    int xPos;
    int yPos;
    SnakeNode* next;
    Direction direction;
    Direction prevDirection;

    SnakeNode(int _xPos, int _yPos, SnakeNode* _next, Direction _direction)
        : xPos(_xPos), yPos(_yPos), next(_next), direction(_direction)
    {
        prevDirection = direction;
    };
};

#endif