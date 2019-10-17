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