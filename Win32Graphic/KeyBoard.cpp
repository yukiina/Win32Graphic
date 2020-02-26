#include "KeyBoard.h"

// 给外部使用的接口

bool KeyBoard::IsKeyPressed(const unsigned char code)
{
	return keyState[code];
}

char KeyBoard::ReadChar()
{
	if (charBuffer.size() > 0) {
		char ret = charBuffer.front();
		charBuffer.pop();
		return ret;
	}
	else {
		return 0;
	}
}

void KeyBoard::FlushChar()
{
	charBuffer = std::queue<char>();
}


// 供 MsgProc 使用的接口

// 当MsgProc检测键按下时, 调用这个方法, keyState对应键为 true, 外部调用IsKeyPressed() 会返回true
void KeyBoard::OnKeyPressed(const unsigned char code)
{
	keyState[code] = true;
}

void KeyBoard::OnKeyReleased(const unsigned char code)
{
	keyState[code] = false;
}

void KeyBoard::OnChar(const char character)
{
	charBuffer.push(character);
	while (charBuffer.size() > nBuffer) {
		charBuffer.pop();
	}
}

void KeyBoard::ClearState()
{
	keyState.reset();
}
