#include<Inferno.h>
#include"TestGame.h"
#include<stdexcept>

using namespace std;
using namespace Inferno;

INT WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		TestGame game;
		game.SetTitle(_T("test"));
		game.Resize(1280, 720);
		game.SetWindowAlignment(HorizontalAlignment::Center, VerticalAlignment::Center);
		game.Show();

		MSG msg;
		while (true)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				//WM_QUITEがきたらGetMessageは0を返す
				if (msg.message == WM_QUIT) break;

				TranslateMessage(&msg); //キーボード関連のメッセージを翻訳する
				DispatchMessage(&msg); //OSにメッセージを渡す（＝ウィンドウプロシージャに渡す）
			}
			else
			{
				//ゲームループ
				game.GameLoop();
			}

			Sleep(1);
		}

		return static_cast<INT>(msg.wParam); //作法
	}
	catch (const std::runtime_error& ex)
	{
		::MessageBoxA(nullptr, ex.what(), "Error", MB_OK | MB_ICONERROR);
	}

	return 0;
}