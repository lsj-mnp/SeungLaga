#include "CGame.h"

LRESULT WINAPI WndProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);

//winbase.h���� ����� ������.
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	CGame Game{ hInstance, 1280, 720 };
	Game.Create(nShowCmd, WndProc, "SeungLaga");
	
	CTexture* texMainShip{ Game.AddTexture(L"Asset/flight.png") };
	CTexture* texBG{ Game.AddTexture(L"Asset/bg.png") };

	CObject2D* objMainShip{ Game.AddObject2D(XMFLOAT2(205.0f, 126.0f)) };
	CObject2D* objBG{ Game.AddObject2D(XMFLOAT2(800.0f, 600.0f)) };

	CGameObject2D* goBG{ Game.AddGameObject2D() };
	goBG->SetSampler(ESampler::LinearWrap);
	goBG->SetObject2D(objBG);
	goBG->SetTexture(texBG);

	CGameObject2D* goMainShip{ Game.AddGameObject2D() };
	goMainShip->SetSampler(ESampler::LinearWrap);
	goMainShip->SetObject2D(objMainShip);
	goMainShip->SetTexture(texMainShip);	

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
			const Keyboard::State& KeyState{ Game.GetKeyboardState() };

			if (KeyState.Up)
			{
				goMainShip->Translate(XMVectorSet(0.0f, +0.001f, 0.0f, 0.0f));

				if (XMVectorGetY(goMainShip->GetTranslation()) >= 1.1f)
				{
					goMainShip->SetTranslationY(-1.1f);
				}
			}
			
			if (KeyState.Down)
			{
				goMainShip->Translate(XMVectorSet(0.0f, -0.001f, 0.0f, 0.0f));

				if (XMVectorGetY(goMainShip->GetTranslation()) <= -1.1f)
				{
					goMainShip->SetTranslationY(1.1f);
				}
			}

			if (KeyState.Left)
			{
				goMainShip->Translate(XMVectorSet(-0.001f, 0.0f, 0.0f, 0.0f));

				if (XMVectorGetX(goMainShip->GetTranslation()) <= -1.1f)
				{
					goMainShip->SetTranslationX(1.1f);
				}
			}

			if (KeyState.Right)
			{
				goMainShip->Translate(XMVectorSet(+0.001f, 0.0f, 0.0f, 0.0f));

				if (XMVectorGetX(goMainShip->GetTranslation()) >= 1.1f)
				{
					goMainShip->SetTranslationX(-1.1f);
				}
			}

			if (KeyState.Escape)
			{
				DestroyWindow(Game.GethWnd());

				continue;
			}

			Game.BeginRendering(color);

			Game.Draw();

			Game.EndRendering();
		}
	}

	return 0;
}

//WIndow Procedure. �޽����� ó���ϴ� �Լ�.
LRESULT WINAPI WndProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (Msg)
	{
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(Msg, wParam, lParam);
		break;
	case WM_ACTIVATEAPP:
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(Msg, wParam, lParam);
		break;
		//���� ��ɾ �޾��� ���.
	case WM_DESTROY:
		//WM_QUIT �޽����� ������.
		PostQuitMessage(0);

		return 0;
		//Ű���带 ������ ���.
	default:
		break;
	}

	//�Ϲ������� ó���� �ʿ䰡 �ִ� �޽����� ��� �⺻���� ó���� ����.
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}