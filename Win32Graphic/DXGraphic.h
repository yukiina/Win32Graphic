#pragma once

/*
 * DirectX 11.0 API封装
 */

#include <wrl.h>
#include <d3d11.h>
#include <string>
#include <DirectXMath.h>
#include "Graphic.h"

using namespace DirectX;
template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

class DXGraphic : public Graphic
{
public:
	DXGraphic(HWND hWnd, float width, float height);
	~DXGraphic();
public:
	virtual void EndFrame() override;          // 前后备缓冲区交换并呈现
	virtual void ClearBuffer(float red, float green, float blue) override; // 负责用指定颜色清除颜色缓冲
	virtual float AspectRatio() override;
	virtual void DrawIndex(int indexCount) override;


private:
	ComPtr<ID3D11Device> pDevice;               // D3D11 设备
	ComPtr<IDXGISwapChain> pSwap;               // D3D11 交换链
	ComPtr<ID3D11DeviceContext> pContext;       // D3D11 设备上下文
	ComPtr<ID3D11RenderTargetView> pTargetView; // D3D11 渲染目标视图
	ComPtr<ID3D11DepthStencilView> pDepthStencilView; // D3D11 深度模板视图
	ComPtr<ID3D11DepthStencilState> DSSLessEqual;     // 深度测试状态
private:
	float width;
	float height;
};

