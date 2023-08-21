#pragma once
#include <random>

namespace Random // capital R to avoid conflicts with functions named random()
{
	extern std::random_device rd;
	extern std::seed_seq ss;
	extern std::mt19937 mt;

	int get(int min, int max);
}
