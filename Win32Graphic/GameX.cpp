#include "GameX.h"
#include <DirectXMath.h>
#include <sstream>

using namespace DirectX;

GameX::GameX(WindowX & wnd)
	:wnd(wnd), gfx(wnd.GetGfx())
{
	// 隐藏鼠标, 限制鼠标活动范围
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
			// 绘制前清空颜色, 深度, 模板 缓冲
			wnd.GetGfx().ClearBuffer(0, 1, 0);
			// 绘制的逻辑
			Update();
			// 绘制后交换(拷贝)缓冲, 使画面呈现在屏幕上
			wnd.GetGfx().EndFrame();
		}
	}

	return (int)msg.wParam;
}



// 每一帧的逻辑
void GameX::Update()
{
	// 时间, 鼠标输入
	float dt = timer.Mark();
	auto raw = wnd.mouse.GetRawDelta();
}
