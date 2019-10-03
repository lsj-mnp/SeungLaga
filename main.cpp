#include "CDirectX.h"
#include "CShader.h"
#include "CObject2D.h"
#include "CTexture.h"
#include "CSamplerState.h"

LRESULT WINAPI WndProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);

//winbase.h���� ����� ������.
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

	//�޼���
	MSG Msg{};

	const float color[]{ 0.3f, 0.2f, 0.7f, 1.0f };

	while (true)
	{
		//�޽����� ������ �޽����� ó���ϴ� ��ũ�� �Լ�. �޽����� ��� true�� �����Ѵ�.
		//hWnd�� nulptr �� �ص־� �� �Լ��� ȣ���� �����忡 ���޵� ��� �޽����� �����Ѵ�.
		//wMsgFilterMin, Max�� ������ �޽����� �ּҰ��� �ִ밪. ��� 0�̸� ��� �޽����� �����Ѵ�.
		//wRemoveMsg�� ������ �޽����� ��� ó������ ���Ѵ�.
		if (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
		{
			//������ �޽���. quit�� �������α׷� ���� ��ȣ�̴�.
			if (Msg.message == WM_QUIT)
			{
				break;
			}

			//Ű���� �Է� �޽����� ���α׷����� �� �� �ְ� ó����.
			TranslateMessage(&Msg);

			//�޽����� ������ �޽��� ó���Լ�(WndProc)�� ������.
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

//WIndow Procedure. �޽����� ó���ϴ� �Լ�.
LRESULT WINAPI WndProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (Msg)
	{
		//���� ��ɾ �޾��� ���.
	case WM_DESTROY:
		//WM_QUIT �޽����� ������.
		PostQuitMessage(0);

		return 0;
		//Ű���带 ������ ���.
	case WM_KEYDOWN:
		//VertureKey �ڵ�. �Ʒ��� ���� esc.
		if (wParam == VK_ESCAPE)
		{
			//�ش� �����츦 �ı���.
			DestroyWindow(hWnd);
		}
		return 0;
	default:
		break;
	}

	//�Ϲ������� ó���� �ʿ䰡 �ִ� �޽����� ��� �⺻���� ó���� ����.
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}