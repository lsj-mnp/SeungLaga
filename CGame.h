#pragma once

#include "SharedHeader.h"
#include "CConstantBuffer.h"
#include "CGameObject2D.h"
#include "CShader.h"
#include "CObject2D.h"
#include "CTexture.h"
#include "CSamplerState.h"

struct ScbMatrixData
{
	XMMATRIX Projection{};
};

class CGame
{
public:
	CGame(HINSTANCE hInstance, float Width, float Height) :
		m_hInstance{ hInstance }, m_Width{ Width }, m_Height{ Height } {};
	~CGame() {};

public:
	void Create(int nShowCmd, WNDPROC WndProc, const char* WindowName);

	void BeginRendering(const float (&ColorRGBA)[4]);

	void Draw();

	void EndRendering();

	CGameObject2D* AddGameObject2D();

	CGameObject2D* GetGameObject2D(size_t Index);

	CShader* AddShader(EShaderType ShaderType, const wstring& ShaderFileName, const string& EntryPoint);

	CShader* GetShader(size_t Index);

	CTexture* AddTexture(const wstring& FileName);

	CTexture* GetTexture(size_t Index);

	CObject2D* AddObject2D(const XMFLOAT2& RectangleSize);

	CObject2D* GetObject2D(size_t Index);

public:
	ID3D11Device* GetDevicePtr() { return m_Device.Get(); }

	ID3D11DeviceContext* GetDeviceContextPtr() { return m_DeviceContext.Get(); }

	Keyboard::State GetKeyboardState();

	Mouse::State GetMouseState();

private:
	void CreateWin32Window(int nShowCmd, WNDPROC WndProc, const char* WindowName);

	void CreateDirect3DObjects();

private:
	void CreateSwapChain();

	void CreateAndSetRenderTargetView();

	void SetViewport();

	void CreateBaseShaders();

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

	Keyboard m_Keyboard{};
	Mouse m_Mouse{};
	
private:
	ScbMatrixData m_cbMatrixData{};
	unique_ptr<CConstantBuffer> m_cbMatrix{};

private:
	vector<unique_ptr<CGameObject2D>> m_vGameObject2Ds{};
	vector<unique_ptr<CShader>> m_vShaders{};
	vector<unique_ptr<CTexture>> m_vTextures{};
	vector<unique_ptr<CObject2D>> m_vObject2Ds{};
};