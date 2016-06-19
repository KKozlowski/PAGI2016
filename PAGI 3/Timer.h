#pragma once

#include <chrono>
#include <iostream>
#include "main.h"

using namespace std::chrono;
class Timer
{
private:
	milliseconds pastMoment;
	unsigned long long deltaMs;
	float deltaTime;
public:

	Timer()
	{
		pastMoment = duration_cast< milliseconds >(
			system_clock::now().time_since_epoch()
			);
	}

	void update()
	{
		milliseconds ms = duration_cast< milliseconds >(
			system_clock::now().time_since_epoch()
			);
		deltaMs = ms.count() - pastMoment.count();
		deltaTime = (float)deltaMs / 1000.0f;
		pastMoment = ms;
	}

	float get_delta_time() const
	{
		return deltaTime;
	}
};
