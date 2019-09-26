#pragma once
#include <Windows.h>

class CDirectX
{
public:
	CDirectX(HINSTANCE hInstance, float Width, float Height) :
		m_hInstance{ hInstance }, m_Width{ Width }, m_Height{ Height } {};
	~CDirectX() {};

	void Create(int nShowCmd, WNDPROC WndProc, const char* WindowName);

private:
	HWND m_hWnd{};
	HINSTANCE m_hInstance{};

	float m_Width{};
	float m_Height{};
};