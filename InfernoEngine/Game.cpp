#include"Game.h"
#include"Graphics.h"
#include"DpiUtility.h"

using namespace Inferno;
using Base = SimpleWindow;

Game* s_pGame;
Input* s_pInputHandle;
GraphicsDirectX11* s_pGraphics;
LRESULT WINAPI MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Game::Game() : m_elapsedFrame(0), m_frameCount(0)
{
	s_pInputHandle = &m_input;
	s_pGame = this;

	//�E�B�h�E�v���V�[�W������������
	::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)MessageHandler);

	//IME������
	m_input.DisableIME(m_hWnd);

	//�O���t�B�b�N�X�̏�����
	m_pGraphics = std::make_unique<GraphicsDirectX11>(m_hWnd);
	// �n���h���Ŏg�����߂Ɏ����Ă���
	s_pGraphics = m_pGraphics.get();

	//�^�C�}�[���X�^�[�g
	m_gameTimer.Start();
	m_previousFrameTimer.Start();
}

Game::~Game()
{
	m_input.EnableIME(m_hWnd);

	s_pInputHandle = nullptr;
}

bool Game::GameLoop()
{
	//�t���[���Ǘ�
	static long long int previousFrame = 0;
	m_frameCount = m_elapsedFrame - previousFrame;
	m_elapsedFrame += 1;
	previousFrame = m_elapsedFrame;
	//�K��t���[�������Ȃ�҂�
	while (m_previousFrameTimer.GetElapsed() < 1000.0 / m_TargetFps)
	{
		Sleep(1);
	}

	//�I��钼�O�Ń^�C�}�[���Z�b�g
	m_previousFrameTimer.Start();
	return true;
}

void Game::Exit()
{
	// DirectX�̓t���X�N���[���ŏI���������ł��Ȃ��̂ŁA�E�B���h�E�ɐ؂�ւ���
	m_pGraphics->SetFullScreenState(false);

	Destroy();
}

void Game::Resize(const int logicalClientWidth, const int logicalClientHeight)
{
	Base::Resize(logicalClientWidth, logicalClientHeight);
	
	//�o�b�t�@�T�C�Y��ύX����
	m_pGraphics->RecreateScreen(m_hWnd, GetDpiScaling());
}

void Game::SampleFps()
{
	//���݂̑��o�߃t���[��
	const auto elapsedTime = m_gameTimer.GetElapsed();

	//���Ԃ��o����
	m_previousFrameTimeStorage.push_back(elapsedTime);
	//�K�萔�ȏソ�܂�����擪���폜
	while (m_previousFrameTimeStorage.size() > PreviousFrameTimeSampleCount)
		m_previousFrameTimeStorage.pop_front();
}

const float Game::GetFps() const
{
	//�����T���v�����O����Ă��Ȃ��ꍇ�A�A��
	if (m_previousFrameTimeStorage.empty()) { return 0.0; }

	//���݂̑��o�߃t���[��
	const auto elapsedTime = m_gameTimer.GetElapsed();

	//�擪�ƍŐV�̒l���r�B
	const auto timeSampleInterval = elapsedTime - m_previousFrameTimeStorage.front();
	//�T���v�����Ŋ���Ε��ς��ł�
	const float intervalAvarage = timeSampleInterval / static_cast<float>(PreviousFrameTimeSampleCount);

	//fps
	const float fps = 1000.0f / intervalAvarage;

	return fps;
}

void Game::SetFullScreenState(bool isFullscreen)
{
	m_pGraphics->SetFullScreenState(isFullscreen);
}

LRESULT WINAPI MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ERASEBKGND:
		//����
		return TRUE;

	case WM_MENUCHAR:
		// Alt + Enter����Beep�𕕂���
		return(MNC_CLOSE << 16);
		break;

	case WM_KEYUP:
		//Esc�L�[�ŃQ�[���I��
		if (wParam == VK_ESCAPE)
		{
			//�t���X�N���[���Ń_�C�����O�͏o����Ȃ��̂ŁA�����I�ɃE�B���h�E�ɐ؂�ւ���B
			s_pGame->SetFullScreenState(false);
			const auto response = ::MessageBox(hWnd, _T("�I�����܂����H"), _T("�m�F"), MB_YESNO | MB_ICONWARNING);
			if (response == IDYES)
			{
				PostQuitMessage(0);
			}
		}
		break;

	case WM_DPICHANGED:
		{
			const RECT* rect = (RECT*)lParam;
			const float dpi = HIWORD(wParam);
			const int width = rect->right - rect->left;
			const int height = rect->bottom - rect->top;
			SetWindowPos(hWnd, HWND_TOP, rect->left, rect->top, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
			//RedrawWindow(hWnd, nullptr, nullptr, RDW_UPDATENOW | RDW_INVALIDATE);
			if (s_pGraphics)
			{
				s_pGraphics->RecreateScreen(hWnd, dpi / USER_DEFAULT_SCREEN_DPI);
				//�؂�ւ����ɕ`��
				s_pGraphics->Draw();
			}
		}
		break;

	default:
		break;
	}

	//���͂�����
	s_pInputHandle->InputProc(hWnd, msg, wParam, lParam);

	//SimpleWindow�̃v���V�[�W����ʂ�
	return Inferno::SimpleWindowProc(hWnd, msg, wParam, lParam);
}
