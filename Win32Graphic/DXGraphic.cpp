#include "DXGraphic.h"
#include "HRException.h"

#pragma comment(lib, "d3d11.lib")

// Debug模式下打开 D3D11_CREATE_DEVICE_DEBUG
#ifndef NDEBUG
UINT debugMode = D3D11_CREATE_DEVICE_DEBUG;
#else
UINT debugMode = 0;
#endif // !NDEBUG



DXGraphic::DXGraphic(HWND hWnd, float width, float height)
	:width(width),
	height(height)
{
	// 交换链描述
	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferDesc.Width = (UINT)width;
	sd.BufferDesc.Height = (UINT)height;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;  // 多重采样不开启
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Buffer的用途是创建后备缓冲
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 后备缓冲使用拷贝模式
	sd.Flags = 0;



	// 创建 设备, 设备上下文, 交换链
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		debugMode,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	);

	if (FAILED(hr)) {
		//TODO: Ex
		HR_ERROR(hr);
	}

	// 获取交换链的后备缓冲区, 用于创建 TargetView
	ComPtr<ID3D11Resource> pBackBuffer;
	hr = pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	if (FAILED(hr)) {
		//TODO: Ex
		HR_ERROR(hr);
	}

	// 为后备缓冲区创建渲染目标视图
	hr = pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTargetView);
	if (FAILED(hr)) {
		//TODO: Ex
		HR_ERROR(hr);
	}


	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// 允许使用深度值一致的像素进行替换的深度/模板状态
	// 该状态用于绘制天空盒，因为深度值为1.0时默认无法通过深度测试
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	dsDesc.StencilEnable = false;

	pDevice->CreateDepthStencilState(&dsDesc, &DSSLessEqual);
	if (FAILED(hr)) {
		//TODO: Ex
		HR_ERROR(hr);
	}

	// 创建深度/模板缓冲, 是一个和后备缓冲大小相同的Texture2D
	ComPtr<ID3D11Texture2D> pDepthStencilBuffer;
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = (UINT)width;
	texDesc.Height = (UINT)height;
	texDesc.Format = DXGI_FORMAT_D32_FLOAT;
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.MipLevels = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = pDevice->CreateTexture2D(&texDesc, nullptr, &pDepthStencilBuffer);
	if (FAILED(hr)) {
		//TODO: Ex
		HR_ERROR(hr);
	}


	// 创建 深度/模板 视图
	hr = pDevice->CreateDepthStencilView(pDepthStencilBuffer.Get(), nullptr, &pDepthStencilView);
	if (FAILED(hr)) {
		//TODO: Ex
		HR_ERROR(hr);
	}

	// 设置深度缓冲状态
	pContext->OMSetDepthStencilState(DSSLessEqual.Get(), 0);

	// 将后备缓冲, 深度缓冲绑定到输出合并阶段的渲染管线
	pContext->OMSetRenderTargets(1u, pTargetView.GetAddressOf(), pDepthStencilView.Get());


	// 设置视口
	D3D11_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;

	// 将视口信息绑定到光栅化阶段 (可设置多视口)
	pContext->RSSetViewports(1u, &vp);
}

DXGraphic::~DXGraphic()
{
	if (pContext) {
		pContext->ClearState();
	}
}

void DXGraphic::EndFrame()
{
	HRESULT hr;
	// 将后备缓冲的信息呈现在屏幕上
	if (FAILED(hr = pSwap->Present(0, 0))) {
		if (hr == ERROR_DEVICE_REMOVED) {
			hr = pDevice->GetDeviceRemovedReason();  // 获取设备被移除的原因的错误信息
			// TODO: Ex
			HR_ERROR(hr);
		}
		else {
			// TODO: Ex
			HR_ERROR(hr);
		}
	}
}

void DXGraphic::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue,  1.0f };
	pContext->ClearRenderTargetView(pTargetView.Get(), color); // 用指定颜色清空需要绘制的缓冲区
	pContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u); // 清除深度缓冲
}

float DXGraphic::AspectRatio()
{
	return (float)width / height;
}

void DXGraphic::DrawIndex(int indexCount)
{
	pContext->DrawIndexed(indexCount, 0, 0);
}






