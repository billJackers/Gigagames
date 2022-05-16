#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include <random>

class Randomizer
{
public:
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_int_distribution<> distr;

	Randomizer()
	{
		
	};

	// Bounds are inclusive
	Randomizer(int lowerBound, int upperBound)
	{
		std::mt19937 g(rd());
		gen = g;

		std::uniform_int_distribution<> d(lowerBound, upperBound);
		distr = d;
	};

	~Randomizer()
	{

	};

	int generate()
	{
		return distr(gen);
	};

};

#endif