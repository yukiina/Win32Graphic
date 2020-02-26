#include "DXGraphic.h"
#include "HRException.h"

#pragma comment(lib, "d3d11.lib")

// Debugģʽ�´� D3D11_CREATE_DEVICE_DEBUG
#ifndef NDEBUG
UINT debugMode = D3D11_CREATE_DEVICE_DEBUG;
#else
UINT debugMode = 0;
#endif // !NDEBUG



DXGraphic::DXGraphic(HWND hWnd, float width, float height)
	:width(width),
	height(height)
{
	// ����������
	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferDesc.Width = (UINT)width;
	sd.BufferDesc.Height = (UINT)height;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;  // ���ز���������
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Buffer����;�Ǵ����󱸻���
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // �󱸻���ʹ�ÿ���ģʽ
	sd.Flags = 0;



	// ���� �豸, �豸������, ������
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

	// ��ȡ�������ĺ󱸻�����, ���ڴ��� TargetView
	ComPtr<ID3D11Resource> pBackBuffer;
	hr = pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	if (FAILED(hr)) {
		//TODO: Ex
		HR_ERROR(hr);
	}

	// Ϊ�󱸻�����������ȾĿ����ͼ
	hr = pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTargetView);
	if (FAILED(hr)) {
		//TODO: Ex
		HR_ERROR(hr);
	}


	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// ����ʹ�����ֵһ�µ����ؽ����滻�����/ģ��״̬
	// ��״̬���ڻ�����պУ���Ϊ���ֵΪ1.0ʱĬ���޷�ͨ����Ȳ���
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	dsDesc.StencilEnable = false;

	pDevice->CreateDepthStencilState(&dsDesc, &DSSLessEqual);
	if (FAILED(hr)) {
		//TODO: Ex
		HR_ERROR(hr);
	}

	// �������/ģ�建��, ��һ���ͺ󱸻����С��ͬ��Texture2D
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


	// ���� ���/ģ�� ��ͼ
	hr = pDevice->CreateDepthStencilView(pDepthStencilBuffer.Get(), nullptr, &pDepthStencilView);
	if (FAILED(hr)) {
		//TODO: Ex
		HR_ERROR(hr);
	}

	// ������Ȼ���״̬
	pContext->OMSetDepthStencilState(DSSLessEqual.Get(), 0);

	// ���󱸻���, ��Ȼ���󶨵�����ϲ��׶ε���Ⱦ����
	pContext->OMSetRenderTargets(1u, pTargetView.GetAddressOf(), pDepthStencilView.Get());


	// �����ӿ�
	D3D11_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;

	// ���ӿ���Ϣ�󶨵���դ���׶� (�����ö��ӿ�)
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
	// ���󱸻������Ϣ��������Ļ��
	if (FAILED(hr = pSwap->Present(0, 0))) {
		if (hr == ERROR_DEVICE_REMOVED) {
			hr = pDevice->GetDeviceRemovedReason();  // ��ȡ�豸���Ƴ���ԭ��Ĵ�����Ϣ
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
	pContext->ClearRenderTargetView(pTargetView.Get(), color); // ��ָ����ɫ�����Ҫ���ƵĻ�����
	pContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u); // �����Ȼ���
}

float DXGraphic::AspectRatio()
{
	return (float)width / height;
}

void DXGraphic::DrawIndex(int indexCount)
{
	pContext->DrawIndexed(indexCount, 0, 0);
}






