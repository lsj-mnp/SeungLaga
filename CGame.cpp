#include "CGame.h"

//������� Direct3DObjects�� �����ϴ� ������ ��.
void CGame::Create(int nShowCmd, WNDPROC WndProc, const char* WindowName)
{
	CreateWin32Window(nShowCmd, WndProc, WindowName);

	CreateDirect3DObjects();

	m_cbProjectionData.Projection = XMMatrixTranspose(XMMatrixOrthographicLH(m_Width, m_Height, 0, 1));

	m_cbProjection = make_unique<CConstantBuffer>(m_Device.Get(), m_DeviceContext.Get());
	m_cbProjection->Create(EShaderType::VertexShader, sizeof(ScbProjectionData), 0);
	m_cbProjection->Update(&m_cbProjectionData);

	m_cbWorld = make_unique<CConstantBuffer>(m_Device.Get(), m_DeviceContext.Get());
	m_cbWorld->Create(EShaderType::VertexShader, sizeof(ScbWorldData), 1);
}

Keyboard::State CGame::GetKeyboardState()
{
	return m_Keyboard.GetState();
}

Mouse::State CGame::GetMouseState()
{
	return m_Mouse.GetState();
}

HWND CGame::GethWnd()
{
	return m_hWnd;
}

//������ ����.
void CGame::CreateWin32Window(int nShowCmd, WNDPROC WndProc, const char* WindowName)
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

void CGame::CreateDirect3DObjects()
{
	CreateSwapChain();

	CreateAndSetRenderTargetView();

	SetViewport();

	CreateBaseShaders();

	m_CommonStates = make_unique<CommonStates>(m_Device.Get());

	m_Mouse.SetWindow(m_hWnd);
	m_Mouse.SetMode(Mouse::MODE_ABSOLUTE);
}

void CGame::BeginRendering(const float(&ColorRGBA)[4])
{
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), ColorRGBA);

	m_DeviceContext->OMSetBlendState(m_CommonStates->NonPremultiplied(), nullptr, 0xFFFFFFFF);
}

void CGame::Draw()
{
	ID3D11SamplerState* PtrSamplerLinearWrap{ m_CommonStates->LinearWrap() };
	ID3D11SamplerState* PtrSamplerPointWrap{ m_CommonStates->PointWrap() };

	m_vShaders[0]->Use();
	m_vShaders[1]->Use();

	m_cbProjection->Use();

	for (auto& GameObject2D : m_vGameObject2Ds)
	{
		switch (GameObject2D->GetSampler())
		{
		case ESampler::LinearWrap:
			m_DeviceContext->PSSetSamplers(0, 1, &PtrSamplerLinearWrap);
			break;
		case ESampler::PointWrap:
			m_DeviceContext->PSSetSamplers(0, 1, &PtrSamplerPointWrap);
			break;
		default:
			break;
		}

		m_cbWorldData.World = XMMatrixTranspose(GameObject2D->GetWorldMatrix());
		m_cbWorld->Update(&m_cbWorldData);
		m_cbWorld->Use();

		GameObject2D->Draw();
	}
}

void CGame::EndRendering()
{
	m_SwapChain->Present(0, 0);
}

CGameObject2D* CGame::AddGameObject2D()
{
	m_vGameObject2Ds.emplace_back(make_unique<CGameObject2D>());
	return m_vGameObject2Ds.back().get();
}

CGameObject2D* CGame::GetGameObject2D(size_t Index)
{
	if (Index >= m_vGameObject2Ds.size()) { return nullptr; }

	return m_vGameObject2Ds[Index].get();
}

CShader* CGame::AddShader(EShaderType ShaderType, const wstring& ShaderFileName, const string& EntryPoint)
{
	m_vShaders.emplace_back(make_unique<CShader>(m_Device.Get(), m_DeviceContext.Get()));
	m_vShaders.back()->Create(ShaderType, ShaderFileName, EntryPoint);

	return m_vShaders.back().get();
}

CShader* CGame::GetShader(size_t Index)
{
	if (Index >= m_vShaders.size()) { return nullptr; }

	return m_vShaders[Index].get();
}

CTexture* CGame::AddTexture(const wstring& FileName)
{
	m_vTextures.emplace_back(make_unique<CTexture>(m_Device.Get(), m_DeviceContext.Get()));
	m_vTextures.back()->CreateFromFile(FileName);
	return m_vTextures.back().get();
}

CTexture* CGame::GetTexture(size_t Index)
{
	if (Index >= m_vTextures.size()) { return nullptr; }

	return m_vTextures[Index].get();
}

CObject2D* CGame::AddObject2D(const XMFLOAT2& RectangleSize)
{
	m_vObject2Ds.emplace_back(make_unique<CObject2D>(m_Device.Get(), m_DeviceContext.Get()));
	m_vObject2Ds.back()->CreateRectangle(RectangleSize);
	return m_vObject2Ds.back().get();
}

CObject2D* CGame::GetObject2D(size_t Index)
{
	if (Index >= m_vObject2Ds.size()) { return nullptr; }

	return m_vObject2Ds[Index].get();
}

void CGame::CreateSwapChain()
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

void CGame::CreateAndSetRenderTargetView()
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

//������ ������ ���� ���ϴ� �Լ�. ȭ�鿡 ���̴� ������ �ʼ���.
void CGame::SetViewport()
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

void CGame::CreateBaseShaders()
{
	AddShader(EShaderType::VertexShader, L"VertexShader.hlsl", "main");
	AddShader(EShaderType::PixelShader, L"PixelShader.hlsl", "main");
}
