#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#include <cassert>

#pragma comment(lib, "d3d11.lib")

using namespace Microsoft::WRL;

class CDirectX
{
public:
	CDirectX(HINSTANCE hInstance, float Width, float Height) :
		m_hInstance{ hInstance }, m_Width{ Width }, m_Height{ Height } {};
	~CDirectX() {};

	void Create(int nShowCmd, WNDPROC WndProc, const char* WindowName);

	void BeginRendering(const float (&ColorRGBA)[4]);
	void EndRendering();

private:
	void CreateWin32Window(int nShowCmd, WNDPROC WndProc, const char* WindowName);

	void CreateDirect3DObjects();

private:
	void CreateSwapChain();

	void CreateAndSetRenderTargetView();

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