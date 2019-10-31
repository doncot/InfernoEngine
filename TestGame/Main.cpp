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
				//WM_QUITE��������GetMessage��0��Ԃ�
				if (msg.message == WM_QUIT) break;

				TranslateMessage(&msg); //�L�[�{�[�h�֘A�̃��b�Z�[�W��|�󂷂�
				DispatchMessage(&msg); //OS�Ƀ��b�Z�[�W��n���i���E�B���h�E�v���V�[�W���ɓn���j
			}
			else
			{
				//�Q�[�����[�v
				game.GameLoop();
			}

			Sleep(1);
		}

		return static_cast<INT>(msg.wParam); //��@
	}
	catch (const std::runtime_error& ex)
	{
		::MessageBoxA(nullptr, ex.what(), "Error", MB_OK | MB_ICONERROR);
	}

	return 0;
}