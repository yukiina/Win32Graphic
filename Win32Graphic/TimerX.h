#pragma once
#include <chrono>

class TimerX
{
public:
	TimerX();
	TimerX(const TimerX&) = delete;
	TimerX& operator=(const TimerX&) = delete;
public:
	float Mark();
private:
	std::chrono::steady_clock::time_point last;
};

