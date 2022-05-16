#ifndef COORDINATE_H
#define COORDINATE_H

struct Coordinate
{
	int x;
	int y;

	Coordinate()
		: x(0), y(0)
	{

	};

	Coordinate(int _x, int _y)
		: x(_x), y(_y)
	{

	};

	~Coordinate()
	{
		
	}

	bool collision(Coordinate c2)
	{
		return this->x == c2.x && this->y == c2.y;
	};
};

#endif