#include "CDirectX.h"
#include "CShader.h"
#include "CObject2D.h"

LRESULT WINAPI WndProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	CDirectX DX{ hInstance, 900, 500 };
	DX.Create(nShowCmd, WndProc, "SeungLaga");

	CShader VS{ DX.GetDevicePtr(), DX.GetDeviceContextPtr() };
	CShader PS{ DX.GetDevicePtr(), DX.GetDeviceContextPtr() };
	VS.Create(EShaderType::Vertex, L"VertexShader.hlsl", "main");
	PS.Create(EShaderType::Pixel, L"PixelShader.hlsl", "main");

	CObject2D OB{ DX.GetDevicePtr(), DX.GetDeviceContextPtr() };

	OB.CreateRectangle(XMFLOAT2(0.5f, 0.5f));

	MSG Msg{};

	const float color[]{ 0.3f, 0.2f, 0.7f, 1.0f };

	while (true)
	{
		if (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (Msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&Msg);

			DispatchMessage(&Msg);
		}
		else
		{
			DX.BeginRendering(color);

			VS.Use();
			PS.Use();

			OB.Draw();

			DX.EndRendering();
		}
	}

	return 0;
}

LRESULT WINAPI WndProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (Msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);

		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hWnd);
		}
		return 0;
	default:
		break;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}