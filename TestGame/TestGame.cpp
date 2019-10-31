#include"TestGame.h"
#include<GameElements.h>
#include<sstream>

using namespace Inferno;
using namespace DirectX;
using Base = Game;

TestGame::TestGame()
{
	m_pGraphics->SetBackgroundColor(0.0f, 0.2f, 0.4f);
}

TestGame::~TestGame()
{
}

bool TestGame::GameLoop()
{
	Base::GameLoop();

	m_pGraphics->LookAt(XMVectorSet(0.0f, 0.0f, -1.25f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));

	if (m_input.IsKeyPressed(VK_F4))
	{
		m_pGraphics->Invert = m_pGraphics->Invert == true ? false : true;
	}

	static long long int frame = 60;
	if (frame < 60)
	{
		frame += 1; SampleFps();
	}
	else
	{
		std::wstringstream ss;
		ss << GetFps();
		m_pGraphics->Text = ss.str();
		//m_pGraphics->Text = std::wstring(L"This is a TEST!!!");
		frame = 0;
	}

	m_pGraphics->Draw();

	return false;
}
