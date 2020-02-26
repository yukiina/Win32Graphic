/*
 *	Win32 Ӧ�ó�����, ʵ���˴��ڵĴ���, ��������, �������, ��������, Raw Input
 *  �������� D3D �� OPenGL �Ŀ���
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