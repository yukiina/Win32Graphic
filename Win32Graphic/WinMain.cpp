/*
 *	Win32 应用程序框架, 实现了窗口的创建, 键盘输入, 按键检测, 鼠标的输入, Raw Input
 *  可以用于 D3D 或 OPenGL 的开发
 */

#include "WindowX.h"
#include "GameX.h"
#include "HRException.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	try {
		WindowX wnd(1280, 720, "Windows");
		GameX game(wnd);
		return game.Run();
	}
	catch (const HRException& e)
	{
		MessageBox(nullptr, e.what(), "HRException", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const ObjModelException& e)
	{
		MessageBox(nullptr, e.what(), "ObjModelException", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}