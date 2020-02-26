#pragma once
#include <Windows.h>
#include <string>
#include <memory>
#include "KeyBoard.h"
#include "Mouse.h"
#include "Graphic.h"

/*
 * Win32窗口的注册和创建
 * WindowClass内部类是窗口注册的抽象, 保存了注册的名称和HINSTANCE
 * WindowX 中管理了图像, 键盘, 鼠标三个类, 供GameX逻辑调用 
 * 内部有Windows Proc 处理消息
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
	// 设置窗口标题
	void SetTitle(const std::string& title);
	// 返回一个图像实例的指针
	Graphic& GetGfx();
	// 隐藏鼠标, 限制鼠标活动范围
	void DisableCursor();
private:
	static LRESULT WINAPI WndXMsgSetUp(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI WndXMsgForward(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	LRESULT WndXMsgHandle(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
public:
	KeyBoard kbd;                  // 键盘实例
	Mouse mouse;                   // 鼠标实例
public:
	std::unique_ptr<Graphic> pGfx; // 图像接口
	HWND hWnd;                     // window 句柄
	int width;                     // 窗口宽度
	int height;                    // 窗口高度
	std::string titleName;         // 窗口标题
};
