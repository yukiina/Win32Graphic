#include "KeyBoard.h"

// ���ⲿʹ�õĽӿ�

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


// �� MsgProc ʹ�õĽӿ�

// ��MsgProc��������ʱ, �����������, keyState��Ӧ��Ϊ true, �ⲿ����IsKeyPressed() �᷵��true
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
