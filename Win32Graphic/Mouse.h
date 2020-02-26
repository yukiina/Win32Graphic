#pragma once
#include <queue>
class Mouse
{
public:
	friend class WindowX;
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
public:
	bool IsLButtonDown();
	bool IsRButtonDown();
	int GetX();
	int GetY();
	int GetWheel();
	std::pair<int, int> GetRawDelta();
// 提供 MsgProc 使用
private:
	void OnMouseMoveRaw(int dx, int dy);
	void OnMouseMove(int x, int y);                    // 鼠标移动
	void OnLButtonDown();                  
	void OnLButtonUp();
	void OnRButtonDown();
	void OnRButtonUp();
	void OnWheelDelta(int delta);       // 滚轮滚动
	void OnWheelUp();                                 // 上滚 
	void OnWheelDown();                               // 下滚
	void ClearState();
private:
	static constexpr unsigned char bufferSize = 16u; // 缓冲区大小
	int x = 0;                      // x 位置
	int y = 0;                      // y 位置 
	std::queue<std::pair<int, int>> rawDeltaBuffer; // raw input 输入缓冲区
	int wheelCount = 0;             // 滚轮累计值(初始为零, 上滚一次 +1, 下滚一次 -1; 可以控制滚轮次数)
	int wheelDelta = 0;             // 滚轮转动角度 -120 ~ 120
	bool leftIsPressed = false;     // LBUTTONDOWN
	bool rightIsPressed = false;    // RBUTTONDOWN
};

