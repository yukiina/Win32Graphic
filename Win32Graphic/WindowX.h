#pragma once
#include <Windows.h>
#include <string>
#include <memory>
#include "KeyBoard.h"
#include "Mouse.h"
#include "Graphic.h"

/*
 * Win32���ڵ�ע��ʹ���
 * WindowClass�ڲ����Ǵ���ע��ĳ���, ������ע������ƺ�HINSTANCE
 * WindowX �й�����ͼ��, ����, ���������, ��GameX�߼����� 
 * �ڲ���Windows Proc ������Ϣ
 */

class WindowX
{
	/***********  WindowClass  *************/
private:
	class WindowClass {
	public:
		static const char* GetName();
		static HINSTANCE GetInst();
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
	private:
		static WindowClass wndClass;
		HINSTANCE hInst;
		static constexpr const char* className = "WindowX Class";

	};
	/***********  WindowClass  *************/
public:
	WindowX(int width = 800, int height = 600, std::string titleName = "Window");
	~WindowX();
	WindowX(const WindowX&) = delete;
	WindowX& operator=(const WindowX&) = delete;

public:
	// ���ô��ڱ���
	void SetTitle(const std::string& title);
	// ����һ��ͼ��ʵ����ָ��
	Graphic& GetGfx();
	// �������, ���������Χ
	void DisableCursor();
private:
	static LRESULT WINAPI WndXMsgSetUp(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI WndXMsgForward(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	LRESULT WndXMsgHandle(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
public:
	KeyBoard kbd;                  // ����ʵ��
	Mouse mouse;                   // ���ʵ��
public:
	std::unique_ptr<Graphic> pGfx; // ͼ��ӿ�
	HWND hWnd;                     // window ���
	int width;                     // ���ڿ��
	int height;                    // ���ڸ߶�
	std::string titleName;         // ���ڱ���
};
