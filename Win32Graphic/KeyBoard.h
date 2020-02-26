#pragma once

#include <queue>
#include <bitset>

// 简易键盘类, 实现检测按键是否按下; 
// 注册事件在WindowsProc 中, 同时保存在一个 bitset 检测对应位是否为 1 
class KeyBoard {
public:
	friend class WindowX;
	KeyBoard() = default;
	KeyBoard(const KeyBoard&) = delete;
	KeyBoard& operator=(const KeyBoard&) = delete;

public:
	bool IsKeyPressed(const unsigned char code);  // 某个键是否按下, 直接输入键名, VK_XXX 
public:
	char ReadChar();
	void FlushChar();
private:
	// MsgProc 使用
	void OnKeyPressed(const unsigned char code);
	void OnKeyReleased(const unsigned char code);
	void OnChar(const char character);
	void ClearState();

private:
	static constexpr unsigned int nState = 256u;
	static constexpr unsigned int nBuffer = 16u;
	std::bitset<nState> keyState;
	std::queue<char> charBuffer;
};

