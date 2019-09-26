#pragma once
#include <Windows.h>

class CDirectX
{
public:
	CDirectX(HINSTANCE hInstance) : m_hInstance{ hInstance } {};
	~CDirectX() {};

	void Create(int nShowCmd, WNDPROC WndProc, const char* WindowName, int Width, int Height);

private:
	HWND m_hWnd{};
	HINSTANCE m_hInstance{};
};