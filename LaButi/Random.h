#pragma once

#include <random>
#include <chrono>

typedef unsigned int uint32;

namespace Random
{
	static std::default_random_engine& Generator()
	{
		static std::default_random_engine generator((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
		return generator;
	}

	// Random roll in the closed interval from, to.
	static uint32 Roll(uint32 from, uint32 to)
	{
		auto &generator = Generator();
		return std::uniform_int_distribution<uint32>(from, to)(generator);
	}

	// Random roll in the closed interval [0, to]. By default [0, 4294967295].
	static uint32 Roll(uint32 to = -1)
	{
		return Roll(0, to);
	}

	// Random roll in the closed interval [from, to].
	static float RollFloat(float from, float to)
	{
		auto &generator = Generator();
		return std::uniform_real_distribution<float>(from, to)(generator);
	}

	// Random roll with real numbers in the closed interval [0, to]. By default [0, 1.0].
	static float RollFloat(float to = 1.0f)
	{
		return RollFloat(0.0f, to);
	}	
	
}
