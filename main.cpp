#include "CDirectX.h"
#include "CShader.h"
#include "CObject2D.h"
#include "CTexture.h"
#include "CSamplerState.h"

LRESULT WINAPI WndProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);

//winbase.h에서 양식을 가져옴.
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	CDirectX DX{ hInstance, 900, 500 };
	DX.Create(nShowCmd, WndProc, "SeungLaga");

	CShader VS{ DX.GetDevicePtr(), DX.GetDeviceContextPtr() };
	CShader PS{ DX.GetDevicePtr(), DX.GetDeviceContextPtr() };
	VS.Create(EShaderType::Vertex, L"VertexShader.hlsl", "main");
	PS.Create(EShaderType::Pixel, L"PixelShader.hlsl", "main");

	CTexture Tex{ DX.GetDevicePtr(), DX.GetDeviceContextPtr() };
	Tex.CreateFromFile(L"Asset/pica.png");

	CTexture TexBG{ DX.GetDevicePtr(), DX.GetDeviceContextPtr() };
	TexBG.CreateFromFile(L"Asset/bg.png");

	CSamplerState Sam{ DX.GetDevicePtr(), DX.GetDeviceContextPtr() };
	Sam.CreateLinear();

	CObject2D OB{ DX.GetDevicePtr(), DX.GetDeviceContextPtr() };
	OB.CreateRectangle(XMFLOAT2(0.4f, 1.0f));

	CObject2D OBBG{ DX.GetDevicePtr(), DX.GetDeviceContextPtr() };
	OBBG.CreateRectangle(XMFLOAT2(1.0f, 1.0f));

	//메세지
	MSG Msg{};

	const float color[]{ 0.3f, 0.2f, 0.7f, 1.0f };

	while (true)
	{
		//메시지가 있으면 메시지를 처리하는 매크로 함수. 메시지가 없어도 true를 리턴한다.
		//hWnd를 nulptr 로 해둬야 이 함수를 호출한 스레드에 전달된 모든 메시지를 조사한다.
		//wMsgFilterMin, Max는 조사할 메시지의 최소값과 최대값. 모두 0이면 모든 메시지를 조사한다.
		//wRemoveMsg는 조사한 메시지를 어떻게 처리할지 정한다.
		if (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
		{
			//윈도우 메시지. quit는 응용프로그램 종료 신호이다.
			if (Msg.message == WM_QUIT)
			{
				break;
			}

			//키보드 입력 메시지를 프로그램에서 쓸 수 있게 처리함.
			TranslateMessage(&Msg);

			//메시지를 윈도우 메시지 처리함수(WndProc)로 전달함.
			DispatchMessage(&Msg);
		}
		else
		{
			DX.BeginRendering(color);

			VS.Use();
			PS.Use();

			Sam.Use();

			TexBG.Use();
			OBBG.Draw();

			Tex.Use();
			OB.Draw();

			DX.EndRendering();
		}
	}

	return 0;
}

//WIndow Procedure. 메시지를 처리하는 함수.
LRESULT WINAPI WndProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (Msg)
	{
		//종료 명령어를 받았을 경우.
	case WM_DESTROY:
		//WM_QUIT 메시지를 전달함.
		PostQuitMessage(0);

		return 0;
		//키보드를 눌렀을 경우.
	case WM_KEYDOWN:
		//VertureKey 코드. 아래의 경우는 esc.
		if (wParam == VK_ESCAPE)
		{
			//해당 윈도우를 파괴함.
			DestroyWindow(hWnd);
		}
		return 0;
	default:
		break;
	}

	//일반적으로 처리될 필요가 있는 메시지에 대란 기본적인 처리를 해줌.
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}