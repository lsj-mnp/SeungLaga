#pragma once

#include "SharedHeader.h"

class CDirectX
{
public:
	CDirectX(HINSTANCE hInstance, float Width, float Height) :
		m_hInstance{ hInstance }, m_Width{ Width }, m_Height{ Height } {};
	~CDirectX() {};

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
	HWND m_hWnd{};
	HINSTANCE m_hInstance{};

	float m_Width{};
	float m_Height{};

	ComPtr<IDXGISwapChain> m_SwapChain{};
	ComPtr<ID3D11Device> m_Device{};
	ComPtr<ID3D11DeviceContext> m_DeviceContext{};
	ComPtr<ID3D11RenderTargetView> m_RenderTargetView{};
};