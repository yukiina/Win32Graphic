#pragma once

#include <queue>
#include <bitset>

// ���׼�����, ʵ�ּ�ⰴ���Ƿ���; 
// ע���¼���WindowsProc ��, ͬʱ������һ�� bitset ����Ӧλ�Ƿ�Ϊ 1 
class KeyBoard {
public:
	friend class WindowX;
	KeyBoard() = default;
	KeyBoard(const KeyBoard&) = delete;
	KeyBoard& operator=(const KeyBoard&) = delete;

public:
	bool IsKeyPressed(const unsigned char code);  // ĳ�����Ƿ���, ֱ���������, VK_XXX 
public:
	char ReadChar();
	void FlushChar();
private:
	// MsgProc ʹ��
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

