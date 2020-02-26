#pragma once

/*
 * DirectX 11.0 API��װ
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
	virtual void EndFrame() override;          // ǰ�󱸻���������������
	virtual void ClearBuffer(float red, float green, float blue) override; // ������ָ����ɫ�����ɫ����
	virtual float AspectRatio() override;
	virtual void DrawIndex(int indexCount) override;


private:
	ComPtr<ID3D11Device> pDevice;               // D3D11 �豸
	ComPtr<IDXGISwapChain> pSwap;               // D3D11 ������
	ComPtr<ID3D11DeviceContext> pContext;       // D3D11 �豸������
	ComPtr<ID3D11RenderTargetView> pTargetView; // D3D11 ��ȾĿ����ͼ
	ComPtr<ID3D11DepthStencilView> pDepthStencilView; // D3D11 ���ģ����ͼ
	ComPtr<ID3D11DepthStencilState> DSSLessEqual;     // ��Ȳ���״̬
private:
	float width;
	float height;
};

