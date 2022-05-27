#ifndef ALIEN_H
#define ALIEN_H

#include "sprite.h"

enum AlienType
{
	NORMAL,
	SHOOTER,
};

class Alien
	: public Sprite
{
public:
	AlienType type;

	Alien(std::vector<float> vertices, std::vector<unsigned int> indices, Shader _shader, Texture _texture, Coordinate startPos, float _width, float _height, AlienType _type)
		: Sprite(vertices, indices, _shader, _texture, startPos, _width, _height), type(_type)
	{
		
	}
};

#endif