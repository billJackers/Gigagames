#ifndef COORDINATE_H
#define COORDINATE_H

struct Coordinate
{
public:
	float x;
	float y;

	Coordinate()
	{

	}

	Coordinate(float _x, float _y)
		: x(_x), y(_y)
	{
	}
};

#endif