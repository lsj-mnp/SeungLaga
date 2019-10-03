#include "CDirectX.h"

//윈도우와 Direct3DObjects를 생성하는 역할을 함.
void CDirectX::Create(int nShowCmd, WNDPROC WndProc, const char* WindowName)
{
	CreateWin32Window(nShowCmd, WndProc, WindowName);

	CreateDirect3DObjects();
}

//윈도우 생성.
void CDirectX::CreateWin32Window(int nShowCmd, WNDPROC WndProc, const char* WindowName)
{
	//윈도우 클래스를 설정하는 구조체.
	WNDCLASSEX wnd_cls{};
	//특별히 안 쓰면 0
	wnd_cls.cbClsExtra = 0;
	//구조체의 크기.
	wnd_cls.cbSize = sizeof(WNDCLASSEX);
	//특별히 안 쓰면 0
	wnd_cls.cbWndExtra = 0;
	//윈도우 바탕색을 칠하는 브러시의 핸들.
	wnd_cls.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	//윈도우가 사용할 마우스 커서의 핸들.
	wnd_cls.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//윈도우가 사용할 아이콘의 핸들.
	wnd_cls.hIcon = wnd_cls.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	//윈도우의 인스턴스의 핸들.
	wnd_cls.hInstance = m_hInstance;
	//윈도우의 메시지 처리함수의 long pointer(핸들).
	wnd_cls.lpfnWndProc = WndProc;
	//윈도우의 클래스 이름.
	wnd_cls.lpszClassName = "Practice";
	//윈도우의 메뉴 이름.
	wnd_cls.lpszMenuName = nullptr;
	//윈도우의 스타일 설정.
	wnd_cls.style = CS_HREDRAW | CS_VREDRAW;

	//클래스 등록용 매크로 함수.
	RegisterClassEx(&wnd_cls);

	/*윈도우 생성.
	1. 확장 스타일 플래그. 안 쓰면 0.
	2. 생성하려는 윈도우 클래스의 이름.
	3. 윈도우의 이름. 윈도우에 표시됨.
	4. 윈도우 초기 스타일 지정. 메뉴, 최소화, 최대화, 종료 버튼 등을 따로 적용할 수도 있다. 해당 경우 기본적인 윈도우 스타일.
	5, 6, 7, 8. 윈도우가 기동했을 때의 위치 및 크기. 해당 경우 기본값. static_cast<바꿀자료형>(대상)은 대상을 바꿀 자료형으로 변환해줌.
	9. 부모 윈도우의 핸들. 해당 경우 본 윈도우 위에 부모가 없으므로 nullptr.
	10. 메뉴의 핸들.
	11. 인스턴스 핸들.
	12. WndProc로 추가적인 데이터를 전달하기 위한 포인터 변수. 안 쓰면 nullptr.*/
	m_hWnd = CreateWindowEx(0, "Practice", WindowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int>(m_Width), static_cast<int>(m_Height), nullptr, nullptr, m_hInstance, nullptr);

	//윈도우의 보이기 상태 지정.
	ShowWindow(m_hWnd, nShowCmd);

	//작업영역을 강제로 그리게 함.
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

	//SwapChain을 설정함.
	DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
	//SwapChain에서 사용할 백버퍼의 갯수. 기본 버퍼를 제외한 백버퍼의 갯수만 적는다.
	swap_chain_desc.BufferCount = 1;
	//생성하려는 백버퍼를 설정함. 맨 위는 포맷의 형식을 설정. 해당 경우는 RGB값 모두 8에 투명도 8까지 해서 32bit. 정규화까지 함.
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//높이.
	swap_chain_desc.BufferDesc.Height = static_cast<UINT>(m_Height);
	//너비.
	swap_chain_desc.BufferDesc.Width = static_cast<UINT>(m_Width);
	//재생률. 해당 경우 1/60.
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	//지정하지 않음.
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	//지정하지 않음.
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	//버퍼의 용도.
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//추가적인 플래그. 안 쓰면 0.
	swap_chain_desc.Flags = 0;
	//결과를 표시할 윈도우의 핸들.
	swap_chain_desc.OutputWindow = m_hWnd;
	//표본의 갯수.
	swap_chain_desc.SampleDesc.Count = 1;
	//표본의 품질.
	swap_chain_desc.SampleDesc.Quality = 0;
	//스왑 이펙트 설정. DISCARD드는 모두 버리고 새로 함. 가장 빠름.
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//윈도우의 기본 모드. 창모드는 TRUE, 전체화면은 FALSE.
	swap_chain_desc.Windowed = TRUE;

	/*디바이스와 SwapChain 생성.
	1. 디스플레이 디바이스의 인터페이스 설정. nullptr일 경우 최초 발견한 디바이스를 사용.
	2. 디바이스 종류 지정. 하드웨어가 가장 빠름.
	3. 소프트웨어의 핸들. 보통은 nullptr
	4. API 레이어를 플래그로 설정. 안 쓸거면 0.
	5. 지원기능레벨의 배열을 설정. nullptr일 경우 촤산 레벨부터 순서대로 설정.
	6. 지원기능의 배열의 갯수. nullptr이니 0.
	7. 현재 사용하는 SDK 버젼을 넘겨줌.
	8. 스왑체인을 설정한 구조체의 포인터.
	9. 스왑체인의 포인터의 포인터.
	10. 디바이스의 포인터의 포인터.
	11. 생성이 성공했을 경우 지원기능레벨에서 지정했던 배열의 처음값을 돌려줌. 실패시 0.
	12. 디바이스 컨텍스트의 포인터의 포인터.*/
	assert(SUCCEEDED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &swap_chain_desc,
		m_SwapChain.GetAddressOf(), m_Device.GetAddressOf(), nullptr, m_DeviceContext.GetAddressOf())));
}

void CDirectX::CreateAndSetRenderTargetView()
{
	assert(!m_RenderTargetView);

	ComPtr<ID3D11Texture2D> back_buffer{};

	//버퍼를 가져옴. 몇 번째 버퍼인지, 범용고유식별자를 통해 어떤 자료형인지, 백버퍼의 포인터의 포인터를 넣음.
	assert(SUCCEEDED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)back_buffer.GetAddressOf())));
	//렌더타겟뷰를 생성. 리소스의 포인터, 렌더타겟뷰를 설정한 구조체의 포인터, 렌더타겟뷰의 포인터의 포인터를 넣음.
	assert(SUCCEEDED(m_Device->CreateRenderTargetView(back_buffer.Get(), nullptr, m_RenderTargetView.GetAddressOf())));

	//파이프라인의 Output merger stage에 렌더타겟뷰를 셋팅함. 렌더타겟의 갯수, 렌더타겟뷰의 포인터의 포인터, 깊이/스탠실 뷰의 포인터를 넣음.
	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);
}

void CDirectX::SetViewport()
{
	//뷰포트 설정. 높이, 너비, 왼쪽 위 X좌표, 왼쪽 위 Y좌표, 깊이값 최소, 최대를 설정함.
	D3D11_VIEWPORT vp{};
	vp.Width = m_Width;
	vp.Height = m_Height;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;

	//파이프라인의 Rasterizer stager에 뷰포트를 설정함. 설정할 뷰포트의 갯수, 뷰포트를 설정한 구조체의 포인터를 넣음.
	m_DeviceContext->RSSetViewports(1, &vp);
}