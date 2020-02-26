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
// �ṩ MsgProc ʹ��
private:
	void OnMouseMoveRaw(int dx, int dy);
	void OnMouseMove(int x, int y);                    // ����ƶ�
	void OnLButtonDown();                  
	void OnLButtonUp();
	void OnRButtonDown();
	void OnRButtonUp();
	void OnWheelDelta(int delta);       // ���ֹ���
	void OnWheelUp();                                 // �Ϲ� 
	void OnWheelDown();                               // �¹�
	void ClearState();
private:
	static constexpr unsigned char bufferSize = 16u; // ��������С
	int x = 0;                      // x λ��
	int y = 0;                      // y λ�� 
	std::queue<std::pair<int, int>> rawDeltaBuffer; // raw input ���뻺����
	int wheelCount = 0;             // �����ۼ�ֵ(��ʼΪ��, �Ϲ�һ�� +1, �¹�һ�� -1; ���Կ��ƹ��ִ���)
	int wheelDelta = 0;             // ����ת���Ƕ� -120 ~ 120
	bool leftIsPressed = false;     // LBUTTONDOWN
	bool rightIsPressed = false;    // RBUTTONDOWN
};

