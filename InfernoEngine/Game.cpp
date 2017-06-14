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

	//ウィドウプロシージャを書き換え
	::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)MessageHandler);

	//IME無効化
	m_input.DisableIME(m_hWnd);

	//グラフィックスの初期化
	m_pGraphics = std::make_unique<GraphicsDirectX11>(m_hWnd);
	// ハンドラで使うために持っておく
	s_pGraphics = m_pGraphics.get();

	//タイマーをスタート
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
	//フレーム管理
	static long long int previousFrame = 0;
	m_frameCount = m_elapsedFrame - previousFrame;
	m_elapsedFrame += 1;
	previousFrame = m_elapsedFrame;
	//規定フレーム未満なら待つ
	while (m_previousFrameTimer.GetElapsed() < 1000.0 / m_TargetFps)
	{
		Sleep(1);
	}

	//終わる直前でタイマーリセット
	m_previousFrameTimer.Start();
	return true;
}

void Game::Exit()
{
	// DirectXはフルスクリーンで終了処理ができないので、ウィンドウに切り替える
	m_pGraphics->SetFullScreenState(false);

	Destroy();
}

void Game::Resize(const int logicalClientWidth, const int logicalClientHeight)
{
	Base::Resize(logicalClientWidth, logicalClientHeight);
	
	//バッファサイズを変更する
	m_pGraphics->RecreateScreen(m_hWnd, GetDpiScaling());
}

void Game::SampleFps()
{
	//現在の総経過フレーム
	const auto elapsedTime = m_gameTimer.GetElapsed();

	//時間を覚える
	m_previousFrameTimeStorage.push_back(elapsedTime);
	//規定数以上たまったら先頭を削除
	while (m_previousFrameTimeStorage.size() > PreviousFrameTimeSampleCount)
		m_previousFrameTimeStorage.pop_front();
}

const float Game::GetFps() const
{
	//一回もサンプリングされていない場合、帰る
	if (m_previousFrameTimeStorage.empty()) { return 0.0; }

	//現在の総経過フレーム
	const auto elapsedTime = m_gameTimer.GetElapsed();

	//先頭と最新の値を比較。
	const auto timeSampleInterval = elapsedTime - m_previousFrameTimeStorage.front();
	//サンプル数で割れば平均がでる
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
		//無視
		return TRUE;

	case WM_MENUCHAR:
		// Alt + Enter時のBeepを封じる
		return(MNC_CLOSE << 16);
		break;

	case WM_KEYUP:
		//Escキーでゲーム終了
		if (wParam == VK_ESCAPE)
		{
			//フルスクリーンでダイヤログは出せれないので、強制的にウィンドウに切り替える。
			s_pGame->SetFullScreenState(false);
			const auto response = ::MessageBox(hWnd, _T("終了しますか？"), _T("確認"), MB_YESNO | MB_ICONWARNING);
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
				//切り替え時に描画
				s_pGraphics->Draw();
			}
		}
		break;

	default:
		break;
	}

	//入力を処理
	s_pInputHandle->InputProc(hWnd, msg, wParam, lParam);

	//SimpleWindowのプロシージャを通す
	return Inferno::SimpleWindowProc(hWnd, msg, wParam, lParam);
}
