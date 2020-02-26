#include "Mouse.h"
#include <Windows.h>

// 外部使用的接口
bool Mouse::IsLButtonDown()
{
	return leftIsPressed;
}

bool Mouse::IsRButtonDown()
{
	return rightIsPressed;
}

int Mouse::GetX()
{
	return x;
}

int Mouse::GetY()
{
	return y;
}

int Mouse::GetWheel()
{
	return wheelCount;
}

std::pair<int, int> Mouse::GetRawDelta()
{
	if (rawDeltaBuffer.empty()) {
		return { 0, 0 };
	}
	auto ret = rawDeltaBuffer.front();
	rawDeltaBuffer.pop();
	return ret;
}






// 内部MsgProc 使用的接口
// 原理和键盘类似, MsgProc 处理消息后, 使用下面的接口设置状态; 使用上边的外部调用读取
void Mouse::OnMouseMoveRaw(int dx, int dy)
{
	// 将移动添加到输入缓冲区, 没有移动时将返回(0, 0)
	rawDeltaBuffer.push({ dx,dy });
	while (rawDeltaBuffer.size() > bufferSize)
	{
		rawDeltaBuffer.pop();
	}
}
void Mouse::OnMouseMove(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Mouse::OnLButtonDown()
{
	leftIsPressed = true;
}

void Mouse::OnLButtonUp()
{
	leftIsPressed = false;
}

void Mouse::OnRButtonDown()
{
	rightIsPressed = true;
}

void Mouse::OnRButtonUp()
{
	rightIsPressed = false;
}

void Mouse::OnWheelDelta(int delta)
{
	wheelDelta += delta;
	while (wheelDelta >= WHEEL_DELTA) {       // WHEEL_DELTA -120 ~ 120
		wheelDelta -= WHEEL_DELTA;
		OnWheelUp();
	}
	while (wheelDelta <= -WHEEL_DELTA) {
		wheelDelta += WHEEL_DELTA;
		OnWheelDown();
	}
}

void Mouse::OnWheelUp()
{
	wheelCount++;
}

void Mouse::OnWheelDown()
{
	wheelCount--;
}

void Mouse::ClearState()
{
	leftIsPressed = false;
	rightIsPressed = false;
}
