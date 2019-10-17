#include "CGame.h"

LRESULT WINAPI WndProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);

//winbase.h에서 양식을 가져옴.
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

//WIndow Procedure. 메시지를 처리하는 함수.
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
		//종료 명령어를 받았을 경우.
	case WM_DESTROY:
		//WM_QUIT 메시지를 전달함.
		PostQuitMessage(0);

		return 0;
		//키보드를 눌렀을 경우.
	default:
		break;
	}

	//일반적으로 처리될 필요가 있는 메시지에 대란 기본적인 처리를 해줌.
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}