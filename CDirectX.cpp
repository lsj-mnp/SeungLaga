#include "CDirectX.h"

void CDirectX::Create(int nShowCmd, WNDPROC WndProc, const char* WindowName)
{
	CreateWin32Window(nShowCmd, WndProc, WindowName);

	CreateDirect3DObjects();
}

void CDirectX::CreateWin32Window(int nShowCmd, WNDPROC WndProc, const char* WindowName)
{
	WNDCLASSEX wnd_cls{};
	wnd_cls.cbClsExtra = 0;
	wnd_cls.cbSize = sizeof(WNDCLASSEX);
	wnd_cls.cbWndExtra = 0;
	wnd_cls.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wnd_cls.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wnd_cls.hIcon = wnd_cls.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wnd_cls.hInstance = m_hInstance;
	wnd_cls.lpfnWndProc = WndProc;
	wnd_cls.lpszClassName = "Practice";
	wnd_cls.lpszMenuName = nullptr;
	wnd_cls.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wnd_cls);

	m_hWnd = CreateWindowEx(0, "Practice", WindowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int>(m_Width), static_cast<int>(m_Height), nullptr, nullptr, m_hInstance, nullptr);

	ShowWindow(m_hWnd, nShowCmd);

	UpdateWindow(m_hWnd);
}

void CDirectX::CreateDirect3DObjects()
{
	CreateSwapChain();

	CreateAndSetRenderTargetView();

	SetViewport();
}

void CDirectX::BeginRendering(const float(&ColorRGBA)[4])
{
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), ColorRGBA);
}

void CDirectX::EndRendering()
{
	m_SwapChain->Present(0, 0);
}

void CDirectX::CreateSwapChain()
{
	assert(!m_SwapChain);

	DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.Height = static_cast<UINT>(m_Height);
	swap_chain_desc.BufferDesc.Width = static_cast<UINT>(m_Width);
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.Flags = 0;
	swap_chain_desc.OutputWindow = m_hWnd;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Windowed = TRUE;

	assert(SUCCEEDED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &swap_chain_desc,
		m_SwapChain.GetAddressOf(), m_Device.GetAddressOf(), nullptr, m_DeviceContext.GetAddressOf())));
}

void CDirectX::CreateAndSetRenderTargetView()
{
	assert(!m_RenderTargetView);

	ComPtr<ID3D11Texture2D> back_buffer{};

	assert(SUCCEEDED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)back_buffer.GetAddressOf())));
	assert(SUCCEEDED(m_Device->CreateRenderTargetView(back_buffer.Get(), nullptr, m_RenderTargetView.GetAddressOf())));

	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);
}

void CDirectX::SetViewport()
{
	D3D11_VIEWPORT vp{};
	vp.Width = m_Width;
	vp.Height = m_Height;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;

	m_DeviceContext->RSSetViewports(1, &vp);
}