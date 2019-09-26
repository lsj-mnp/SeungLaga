#include "CDirectX.h"

void CDirectX::Create(int nShowCmd, WNDPROC WndProc, const char* WindowName)
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
		CW_USEDEFAULT, CW_USEDEFAULT, (int)m_Width, (int)m_Height, nullptr, nullptr, m_hInstance, nullptr);

	ShowWindow(m_hWnd, nShowCmd);

	UpdateWindow(m_hWnd);
}