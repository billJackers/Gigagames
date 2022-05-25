#ifndef COORDINATE_H
#define COORDINATE_H

struct Coordinate
{
public:
	int x;
	int y;

	Coordinate()
	{

	}

	Coordinate(int _x, int _y)
		: x(_x), y(_y)
	{
	}
};

#endif