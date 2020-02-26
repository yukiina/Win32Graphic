#include "TimerX.h"



TimerX::TimerX()
	:last(std::chrono::steady_clock::now())
{
}

float TimerX::Mark()
{
	const auto old = last;
	last = std::chrono::steady_clock::now();
	std::chrono::duration<float> du = last - old;
	return du.count();
}



