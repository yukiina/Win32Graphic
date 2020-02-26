#include "GameX.h"
#include <DirectXMath.h>
#include <sstream>

using namespace DirectX;

GameX::GameX(WindowX & wnd)
	:wnd(wnd), gfx(wnd.GetGfx())
{
	// �������, ���������Χ
	wnd.DisableCursor();

}



int GameX::Run()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// ����ǰ�����ɫ, ���, ģ�� ����
			wnd.GetGfx().ClearBuffer(0, 1, 0);
			// ���Ƶ��߼�
			Update();
			// ���ƺ󽻻�(����)����, ʹ�����������Ļ��
			wnd.GetGfx().EndFrame();
		}
	}

	return (int)msg.wParam;
}



// ÿһ֡���߼�
void GameX::Update()
{
	// ʱ��, �������
	float dt = timer.Mark();
	auto raw = wnd.mouse.GetRawDelta();
}
