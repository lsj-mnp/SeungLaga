#include "CDirectX.h"

//������� Direct3DObjects�� �����ϴ� ������ ��.
void CDirectX::Create(int nShowCmd, WNDPROC WndProc, const char* WindowName)
{
	CreateWin32Window(nShowCmd, WndProc, WindowName);

	CreateDirect3DObjects();
}

//������ ����.
void CDirectX::CreateWin32Window(int nShowCmd, WNDPROC WndProc, const char* WindowName)
{
	//������ Ŭ������ �����ϴ� ����ü.
	WNDCLASSEX wnd_cls{};
	//Ư���� �� ���� 0
	wnd_cls.cbClsExtra = 0;
	//����ü�� ũ��.
	wnd_cls.cbSize = sizeof(WNDCLASSEX);
	//Ư���� �� ���� 0
	wnd_cls.cbWndExtra = 0;
	//������ �������� ĥ�ϴ� �귯���� �ڵ�.
	wnd_cls.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	//�����찡 ����� ���콺 Ŀ���� �ڵ�.
	wnd_cls.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//�����찡 ����� �������� �ڵ�.
	wnd_cls.hIcon = wnd_cls.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	//�������� �ν��Ͻ��� �ڵ�.
	wnd_cls.hInstance = m_hInstance;
	//�������� �޽��� ó���Լ��� long pointer(�ڵ�).
	wnd_cls.lpfnWndProc = WndProc;
	//�������� Ŭ���� �̸�.
	wnd_cls.lpszClassName = "Practice";
	//�������� �޴� �̸�.
	wnd_cls.lpszMenuName = nullptr;
	//�������� ��Ÿ�� ����.
	wnd_cls.style = CS_HREDRAW | CS_VREDRAW;

	//Ŭ���� ��Ͽ� ��ũ�� �Լ�.
	RegisterClassEx(&wnd_cls);

	/*������ ����.
	1. Ȯ�� ��Ÿ�� �÷���. �� ���� 0.
	2. �����Ϸ��� ������ Ŭ������ �̸�.
	3. �������� �̸�. �����쿡 ǥ�õ�.
	4. ������ �ʱ� ��Ÿ�� ����. �޴�, �ּ�ȭ, �ִ�ȭ, ���� ��ư ���� ���� ������ ���� �ִ�. �ش� ��� �⺻���� ������ ��Ÿ��.
	5, 6, 7, 8. �����찡 �⵿���� ���� ��ġ �� ũ��. �ش� ��� �⺻��. static_cast<�ٲ��ڷ���>(���)�� ����� �ٲ� �ڷ������� ��ȯ����.
	9. �θ� �������� �ڵ�. �ش� ��� �� ������ ���� �θ� �����Ƿ� nullptr.
	10. �޴��� �ڵ�.
	11. �ν��Ͻ� �ڵ�.
	12. WndProc�� �߰����� �����͸� �����ϱ� ���� ������ ����. �� ���� nullptr.*/
	m_hWnd = CreateWindowEx(0, "Practice", WindowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int>(m_Width), static_cast<int>(m_Height), nullptr, nullptr, m_hInstance, nullptr);

	//�������� ���̱� ���� ����.
	ShowWindow(m_hWnd, nShowCmd);

	//�۾������� ������ �׸��� ��.
	UpdateWindow(m_hWnd);
}

void CDirectX::CreateDirect3DObjects()
{
	CreateSwapChain();

	CreateAndSetRenderTargetView();

	SetViewport();

	m_CommonStates = make_unique<CommonStates>(m_Device.Get());
}

void CDirectX::BeginRendering(const float(&ColorRGBA)[4])
{
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), ColorRGBA);

	m_DeviceContext->OMSetBlendState(m_CommonStates->NonPremultiplied(), nullptr, 0xFFFFFFFF);
}

void CDirectX::EndRendering()
{
	m_SwapChain->Present(0, 0);
}

void CDirectX::CreateSwapChain()
{
	assert(!m_SwapChain);

	//SwapChain�� ������.
	DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
	//SwapChain���� ����� ������� ����. �⺻ ���۸� ������ ������� ������ ���´�.
	swap_chain_desc.BufferCount = 1;
	//�����Ϸ��� ����۸� ������. �� ���� ������ ������ ����. �ش� ���� RGB�� ��� 8�� ���� 8���� �ؼ� 32bit. ����ȭ���� ��.
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//����.
	swap_chain_desc.BufferDesc.Height = static_cast<UINT>(m_Height);
	//�ʺ�.
	swap_chain_desc.BufferDesc.Width = static_cast<UINT>(m_Width);
	//�����. �ش� ��� 1/60.
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	//�������� ����.
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	//�������� ����.
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	//������ �뵵.
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//�߰����� �÷���. �� ���� 0.
	swap_chain_desc.Flags = 0;
	//����� ǥ���� �������� �ڵ�.
	swap_chain_desc.OutputWindow = m_hWnd;
	//ǥ���� ����.
	swap_chain_desc.SampleDesc.Count = 1;
	//ǥ���� ǰ��.
	swap_chain_desc.SampleDesc.Quality = 0;
	//���� ����Ʈ ����. DISCARD��� ��� ������ ���� ��. ���� ����.
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//�������� �⺻ ���. â���� TRUE, ��üȭ���� FALSE.
	swap_chain_desc.Windowed = TRUE;

	/*����̽��� SwapChain ����.
	1. ���÷��� ����̽��� �������̽� ����. nullptr�� ��� ���� �߰��� ����̽��� ���.
	2. ����̽� ���� ����. �ϵ��� ���� ����.
	3. ����Ʈ������ �ڵ�. ������ nullptr
	4. API ���̾ �÷��׷� ����. �� ���Ÿ� 0.
	5. ������ɷ����� �迭�� ����. nullptr�� ��� �һ� �������� ������� ����.
	6. ��������� �迭�� ����. nullptr�̴� 0.
	7. ���� ����ϴ� SDK ������ �Ѱ���.
	8. ����ü���� ������ ����ü�� ������.
	9. ����ü���� �������� ������.
	10. ����̽��� �������� ������.
	11. ������ �������� ��� ������ɷ������� �����ߴ� �迭�� ó������ ������. ���н� 0.
	12. ����̽� ���ؽ�Ʈ�� �������� ������.*/
	assert(SUCCEEDED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &swap_chain_desc,
		m_SwapChain.GetAddressOf(), m_Device.GetAddressOf(), nullptr, m_DeviceContext.GetAddressOf())));
}

void CDirectX::CreateAndSetRenderTargetView()
{
	assert(!m_RenderTargetView);

	ComPtr<ID3D11Texture2D> back_buffer{};

	//���۸� ������. �� ��° ��������, ��������ĺ��ڸ� ���� � �ڷ�������, ������� �������� �����͸� ����.
	assert(SUCCEEDED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)back_buffer.GetAddressOf())));
	//����Ÿ�ٺ並 ����. ���ҽ��� ������, ����Ÿ�ٺ並 ������ ����ü�� ������, ����Ÿ�ٺ��� �������� �����͸� ����.
	assert(SUCCEEDED(m_Device->CreateRenderTargetView(back_buffer.Get(), nullptr, m_RenderTargetView.GetAddressOf())));

	//������������ Output merger stage�� ����Ÿ�ٺ並 ������. ����Ÿ���� ����, ����Ÿ�ٺ��� �������� ������, ����/���Ľ� ���� �����͸� ����.
	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);
}

void CDirectX::SetViewport()
{
	//����Ʈ ����. ����, �ʺ�, ���� �� X��ǥ, ���� �� Y��ǥ, ���̰� �ּ�, �ִ븦 ������.
	D3D11_VIEWPORT vp{};
	vp.Width = m_Width;
	vp.Height = m_Height;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;

	//������������ Rasterizer stager�� ����Ʈ�� ������. ������ ����Ʈ�� ����, ����Ʈ�� ������ ����ü�� �����͸� ����.
	m_DeviceContext->RSSetViewports(1, &vp);
}