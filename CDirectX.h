#pragma once
#include <Windows.h>

class CDirectX
{
public:
	CDirectX() {};
	~CDirectX() {};

	void Create(HINSTANCE hInstance, int nShowCmd, WNDPROC WndProc)
	{
		WNDCLASSEX wnd_cls{};
		wnd_cls.cbClsExtra = 0;
		wnd_cls.cbSize = sizeof(WNDCLASSEX);
		wnd_cls.cbWndExtra = 0;
		wnd_cls.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
		wnd_cls.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wnd_cls.hIcon = wnd_cls.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		wnd_cls.hInstance = hInstance;
		wnd_cls.lpfnWndProc = WndProc;
		wnd_cls.lpszClassName = "Practice";
		wnd_cls.lpszMenuName = nullptr;
		wnd_cls.style = CS_HREDRAW | CS_VREDRAW;

		RegisterClassEx(&wnd_cls);

		HWND hWnd{ CreateWindowEx(0, "Practice", "window", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 900, 500, nullptr, nullptr, hInstance, nullptr) };

		ShowWindow(hWnd, nShowCmd);

		UpdateWindow(hWnd);
	}

private:

};