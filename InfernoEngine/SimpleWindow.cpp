#include"SimpleWindow.h"
#include"DpiUtility.h"
#include"Shellapi.h"
#include<WinUser.h>

//メモリリーク検出用。
#if defined(_DEBUG)
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	#ifndef DBG_NEW
		#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
	#define new DBG_NEW
	#endif
#endif

LPCTSTR className = TEXT("SimpleWindow");

Inferno::SimpleWindow* s_pGame;

using namespace Inferno;

SimpleWindow::SimpleWindow()
{
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif //_DEBUG

	//プロシージャと通信できるように自分自身をこのスコープで覚えさせる
	s_pGame = this;

	//WindowClass設定
	m_wc.cbSize = sizeof(WNDCLASSEX);
	//CS_CLASSDC:全てのウィンドウが同じデバイスコンテキストを共有するe
	m_wc.style = CS_CLASSDC | CS_HREDRAW | CS_VREDRAW;
	m_wc.lpfnWndProc = SimpleWindowProc;
	m_wc.cbClsExtra = 0L;
	m_wc.cbWndExtra = 0L;
	m_wc.hInstance = GetModuleHandle(nullptr);
	m_wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	m_wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	m_wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	m_wc.lpszMenuName = nullptr;
	m_wc.lpszClassName = className;
	m_wc.hIconSm = nullptr;

	//WindowClass登録
	if (!RegisterClassEx(&m_wc))
	{
		throw std::runtime_error("ウィンドウクラスの登録に失敗しました。");
	}

	//ウィンドウを生成
	m_hWnd = CreateWindow(className, TEXT("SimpleWindow"),
		WindowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, //ウィンドウ位置
		CW_USEDEFAULT, CW_USEDEFAULT, //ウィンドウサイズ
		nullptr, nullptr, m_wc.hInstance, nullptr);
	if (m_hWnd == nullptr)
	{
		::UnregisterClass(className, m_wc.hInstance);
		throw std::runtime_error("ウィンドウの生成に失敗しました。");
	}

	Resize(600, 400);
	SetWindowAlignment(HorizontalAlignment::Center, VerticalAlignment::Center);
}

SimpleWindow::~SimpleWindow()
{
	::UnregisterClass(className, m_wc.hInstance);

#ifdef _DEBUG
	::_CrtDumpMemoryLeaks();
#endif
}

void SimpleWindow::Resize(const int logicalClientWidth, const int logicalClientHeight)
{
	//dpi計算
	const auto dpiScaling = GetDpiScaling();

	const int scaledClientWidth = static_cast<int>(logicalClientWidth * dpiScaling);
	const int scaledClientHeight = static_cast<int>(logicalClientHeight * dpiScaling);

	RECT newWindowSize = {0, 0, scaledClientWidth, scaledClientHeight };
	::AdjustWindowRect(&newWindowSize, WindowStyle, FALSE);

	const int windowWidth = newWindowSize.right - newWindowSize.left;
	const int windowHeight = newWindowSize.bottom - newWindowSize.top;

	// ウインドウサイズの再設定
	SetWindowPos(m_hWnd, HWND_TOP, 0, 0, windowWidth, windowHeight, SWP_NOMOVE);
}

void SimpleWindow::SetWindowAlignment(HorizontalAlignment h, VerticalAlignment v)
{
	//GetWindowPlacementとSetWindowPlacementで
	//位置の保存と復元ができる

	// ウインドウ全体の横幅の幅を計算
	RECT rect;
	::GetWindowRect(m_hWnd, &rect);		// ウインドウ全体のサイズ取得
	const int ww = rect.right - rect.left;	// ウインドウ全体の幅の横幅を計算
	const int wh = rect.bottom - rect.top;	// ウインドウ全体の幅の縦幅を計算

	::GetClientRect(m_hWnd, &rect);
	const int cw = rect.right - rect.left;
	const int ch = rect.bottom - rect.top;

	//result
	int rx = 0;
	int ry = 0;

	//位置を座標に直す
	switch(h)
	{
	case HorizontalAlignment::Center:
		rx = (GetSystemMetrics(SM_CXSCREEN) - ww) / 2;
		if (rx < 0) rx=0; //画面が小さすぎた場合
		break;
	default:
		assert(false);
		break;
	}

	switch(v)
	{
	case VerticalAlignment::Center:
		ry = (GetSystemMetrics(SM_CYSCREEN) - wh) / 2;
		if (ry < 0) ry = 0; //画面が小さすぎた場合
		break;
	default:
		assert(false);
		break;
	}

	//adjust (offset)
	int ax = 0;
	int ay = 0;

	//タスクバーがあった場合調整する
	//タスクバーの位置を矩形で入手
	APPBARDATA sBarInfo;
	ZeroMemory(&sBarInfo, sizeof(APPBARDATA));
	sBarInfo.cbSize = sizeof(APPBARDATA);
	sBarInfo.hWnd = m_hWnd;
	SHAppBarMessage(ABM_GETTASKBARPOS, &sBarInfo);
	RECT r = sBarInfo.rc;
	//タスクバーが左にあった時の対処
	if (r.left <= 0 && r.top <= 0 &&//タスクバーの左端と上端が0以下で
		r.right <= r.bottom) //右辺より下辺が長い場合
	{
		ax = r.right; //タスクバー分だけ領域を減らす
	}
	//もしデスクトップをオーバーした場合は
	if (rx + ax + ww > GetSystemMetrics(SM_CXSCREEN))
	{
		//タスクバー、もしくはデスクトップ端にジャストくっつようにする
		rx = ax - (ww - cw) / 2;  //後ろの値はウィンドウ枠だけズラしている
		ax = 0;
	}

	//タスクバーが上にあった時の対処
	if (r.left <= 0 && r.top <= 0 &&//タスクバーの左端と上端が0以下で
		r.bottom <= r.right) //下辺より右辺が長い場合
	{
		ay = r.bottom;//タスクバー分だけ領域を減らす
	}
	//もしデスクトップをオーバーした場合は
	if (ry + ay + wh > GetSystemMetrics(SM_CYSCREEN))
	{
		ry = ay - (wh - ch) / 2; //タスクバー、もしくはデスクトップ端にジャストくっつようにする
		ay = 0;
	}
	
	::SetWindowPos(m_hWnd, HWND_TOP, rx+ax, ry+ay, 0, 0, SWP_NOSIZE);
}

void SimpleWindow::Show() const
{
	// http://stackoverflow.com/questions/17488826/the-meaning-of-sw-show-and-sw-shownormal
	::ShowWindow(m_hWnd, SW_SHOWNORMAL);

	::ValidateRect(m_hWnd, nullptr); //WM_PAINTメッセージを出して
	::UpdateWindow(m_hWnd); //領域を更新
}

void SimpleWindow::SetTitle(LPCTSTR str) const
{
	::SetWindowText(m_hWnd, str);
}

void SimpleWindow::Destroy()
{
	::DestroyWindow(m_hWnd);
}

LRESULT WINAPI Inferno::SimpleWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return (DefWindowProc(hWnd, msg, wParam, lParam));
}
