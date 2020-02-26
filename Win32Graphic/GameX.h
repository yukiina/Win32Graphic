#pragma once

#include "WindowX.h"
#include "TimerX.h"
#include "Graphic.h"


class GameX
{
public:
	GameX(WindowX& wnd);
	GameX(const GameX&) = delete;
	GameX& operator= (const GameX&) = delete;
public:
	int Run();
private:
	void Update();
private:
	WindowX& wnd;
	TimerX timer;
	Graphic& gfx;

};

