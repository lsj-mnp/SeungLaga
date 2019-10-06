#pragma once

#include "SharedHeader.h"
#include "CConstantBuffer.h"

struct ScbMatrixData
{
	XMMATRIX Projection{};
};

class CDirectX
{
public:
	CDirectX(HINSTANCE hInstance, float Width, float Height) :
		m_hInstance{ hInstance }, m_Width{ Width }, m_Height{ Height } {};
	~CDirectX() {};

public:
	void Create(int nShowCmd, WNDPROC WndProc, const char* WindowName);

	void BeginRendering(const float (&ColorRGBA)[4]);

	void EndRendering();

public:
	ID3D11Device* GetDevicePtr() { return m_Device.Get(); }

	ID3D11DeviceContext* GetDeviceContextPtr() { return m_DeviceContext.Get(); }

private:
	void CreateWin32Window(int nShowCmd, WNDPROC WndProc, const char* WindowName);

	void CreateDirect3DObjects();

private:
	void CreateSwapChain();

	void CreateAndSetRenderTargetView();

	void SetViewport();

private:
	//윈도우 핸들 저장.
	HWND m_hWnd{};
	//실행중인 인스턴스의 핸들 및 정보 저장.
	HINSTANCE m_hInstance{};

	float m_Width{};
	float m_Height{};
	
private:
	//ComPtr. Com객체에 사용하는, 마이크로소프트가 만든 스마트 포인터.

	//버퍼를 교체하는 역할을 함. DXGI는 잘 바뀌지 않는 내용들이 보관되어있음.
	ComPtr<IDXGISwapChain> m_SwapChain{};
	//resource를 생성하는 역할을 함. D3D11는 D3D의 11번째 버젼이라는 뜻.
	ComPtr<ID3D11Device> m_Device{};
	//resource를 사용하는 역할을 함.
	ComPtr<ID3D11DeviceContext> m_DeviceContext{};
	//애니메이션을 어디에 그릴지 정하는 역할을 함.
	ComPtr<ID3D11RenderTargetView> m_RenderTargetView{};
	//기본적인 State들을 가지고 있음.
	unique_ptr<CommonStates> m_CommonStates{};
	
private:
	ScbMatrixData m_cbMatrixData{};
	unique_ptr<CConstantBuffer> m_cbMatrix{};
};