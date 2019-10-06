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
	//������ �ڵ� ����.
	HWND m_hWnd{};
	//�������� �ν��Ͻ��� �ڵ� �� ���� ����.
	HINSTANCE m_hInstance{};

	float m_Width{};
	float m_Height{};
	
private:
	//ComPtr. Com��ü�� ����ϴ�, ����ũ�μ���Ʈ�� ���� ����Ʈ ������.

	//���۸� ��ü�ϴ� ������ ��. DXGI�� �� �ٲ��� �ʴ� ������� �����Ǿ�����.
	ComPtr<IDXGISwapChain> m_SwapChain{};
	//resource�� �����ϴ� ������ ��. D3D11�� D3D�� 11��° �����̶�� ��.
	ComPtr<ID3D11Device> m_Device{};
	//resource�� ����ϴ� ������ ��.
	ComPtr<ID3D11DeviceContext> m_DeviceContext{};
	//�ִϸ��̼��� ��� �׸��� ���ϴ� ������ ��.
	ComPtr<ID3D11RenderTargetView> m_RenderTargetView{};
	//�⺻���� State���� ������ ����.
	unique_ptr<CommonStates> m_CommonStates{};
	
private:
	ScbMatrixData m_cbMatrixData{};
	unique_ptr<CConstantBuffer> m_cbMatrix{};
};