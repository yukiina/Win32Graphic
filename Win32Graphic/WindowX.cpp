#include "WindowX.h"
#include "HRException.h"
#include "DXGraphic.h"
#include "GLGraphic.h"


// wndClass��ʵ��
WindowX::WindowClass WindowX::WindowClass::wndClass;

const char * WindowX::WindowClass::GetName()
{
	return wndClass.className;
}

HINSTANCE WindowX::WindowClass::GetInst()
{
	return wndClass.hInst;
}

// ע�ᴰ��
WindowX::WindowClass::WindowClass() noexcept
	:hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = 0;
	wc.hbrBackground = nullptr;
	wc.hCursor = nullptr;
	wc.hIcon = nullptr;
	wc.hIconSm = nullptr;
	wc.hInstance = hInst;
	wc.lpfnWndProc = WndXMsgSetUp;
	wc.lpszClassName = className;
	wc.lpszMenuName = nullptr;
	wc.style = CS_OWNDC;

	RegisterClassEx(&wc);
}

WindowX::WindowClass::~WindowClass()
{
	UnregisterClass(className, hInst);
}

WindowX::WindowX(int width, int height, std::string titleName)
	:width(width),
	height(height),
	titleName(titleName)
{
	RECT rc;
	rc.left = 0;
	rc.right = rc.left + width;
	rc.top = 0;
	rc.bottom = rc.top + height;

	if (AdjustWindowRect(&rc, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0) {
		HR_ERROR(GetLastError());
	}

	hWnd = CreateWindow(WindowX::WindowClass::GetName(), titleName.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top,
		nullptr, nullptr, WindowClass::GetInst(), this);

	if (hWnd == nullptr) {
		HR_ERROR(GetLastError());
	}

	ShowWindow(hWnd, SW_SHOW);

	//pGfx = std::make_unique<DXGraphic>(hWnd, (float)width, (float)height);
	pGfx = std::make_unique<GLGraphic>(hWnd, (float)width, (float)height);

	// ע��RawInput ��Ϣ
	RAWINPUTDEVICE rid = {};
	rid.usUsagePage = 0x01; // mouse page
	rid.usUsage = 0x02;     // mouse usage
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE) {
		HR_ERROR(GetLastError());
	}

}

WindowX::~WindowX()
{
	DestroyWindow(hWnd);
}

void WindowX::SetTitle(const std::string & title)
{
	SetWindowText(hWnd, title.c_str());
}

Graphic& WindowX::GetGfx()
{
	return *pGfx;
}

// �������, ���������Χ
void WindowX::DisableCursor()
{
	while (ShowCursor(FALSE) >= 0);
	RECT rect;
	GetClientRect(hWnd, &rect);
	MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
}

// ����ֱ�Ӱ󶨳�Ա������Windows Proc, ������Ҫ��һЩת��
// ��Ϣ��������ʼ��,����thisָ�뵽USERDATA,�滻��Ϣ������
LRESULT WINAPI WindowX::WndXMsgSetUp(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (Msg == WM_NCCREATE) {
		// lParam�б�����CreateWindowʱ���ݵ�thisָ��
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);

		WindowX* const pWnd = static_cast<WindowX*>(pCreate->lpCreateParams);

		// �� this ָ�뱣�浽 userdata��
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		// �滻��Ϣ������
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowX::WndXMsgForward));

		return pWnd->WndXMsgHandle(hWnd, Msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

// ���е���Ϣ�������������ת������Ա���� WndXMsgHandle
LRESULT WINAPI WindowX::WndXMsgForward(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	// ȡ�� userdata �б���� this ָ��, ת����Ϣ
	WindowX* const pWnd = reinterpret_cast<WindowX*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->WndXMsgHandle(hWnd, Msg, wParam, lParam);
}

// ʵ�ʵ���Ϣ������
LRESULT WindowX::WndXMsgHandle(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg) {
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	// ������Ϣ����
	case WM_KILLFOCUS: // ʧȥ����, Ӧ���ͷ����б���ļ�
	{
		kbd.ClearState();
		mouse.ClearState();
		break;
	}
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		// ����ESCֱ�ӽ�������
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
			return 0;
		}
		// ֻ�����һ�ΰ��� (��һ�ΰ���lParam��30λΪ0), ������������bitset����������
		if (!(lParam & 0x40000000)) {
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	}
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	}
	case WM_CHAR:
	{
		kbd.OnChar(static_cast<char>(wParam));
		break;
	}
	// Raw Input ���� (���)
	case WM_INPUT:
	{
		UINT dataSize;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT,
			nullptr, &dataSize, sizeof(RAWINPUTHEADER));

		if (dataSize > 0)
		{
			std::unique_ptr<char[]> rawdata = std::make_unique<char[]>(dataSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT,
				rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
				if (raw->header.dwType == RIM_TYPEMOUSE) {
					mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
			}
		}
		break;
	}
	// �����Ϣ����
	case WM_MOUSEMOVE: // ����ƶ�
	{
		POINTS pt = MAKEPOINTS(lParam);
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height) {
			mouse.OnMouseMove(pt.x, pt.y);
		}
		break;
	}
	case WM_LBUTTONDOWN: // �������
	{
		POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLButtonDown();
		break;
	}
	case WM_LBUTTONUP:
	{
		POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLButtonUp();
		break;
	}
	case WM_RBUTTONDOWN:
	{
		POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRButtonDown();
		break;
	}
	case WM_RBUTTONUP:
	{
		POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRButtonUp();
		break;
	}

	case WM_MOUSEWHEEL:  // ���ֹ���
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(delta);
		break;
	}
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}
