#include "Mouse.h"
#include <Windows.h>

// �ⲿʹ�õĽӿ�
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






// �ڲ�MsgProc ʹ�õĽӿ�
// ԭ��ͼ�������, MsgProc ������Ϣ��, ʹ������Ľӿ�����״̬; ʹ���ϱߵ��ⲿ���ö�ȡ
void Mouse::OnMouseMoveRaw(int dx, int dy)
{
	// ���ƶ���ӵ����뻺����, û���ƶ�ʱ������(0, 0)
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
