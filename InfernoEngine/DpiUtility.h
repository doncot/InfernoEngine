#pragma once
#include "WindowsCommon.h"

namespace Inferno {

	/// <summary>
	/// DPIを取得する
	/// </summary>
	/// <param name="hWnd">Per-Monitor Awareの場合、渡したウィンドウのあるスクリーンのdpiを取得する。それ以外の場合は無視される。</param>
	/// <remarks>Per-Monitor Awareの場合、現在配置されているウィンドウのDPIを返す。</remarks>
	float GetDpi(HWND hWnd = nullptr);

	/// <summary>
	/// DPIスケーリングを取得する
	/// </summary>
	/// <param name="hWnd">Per-Monitor Awareの場合、渡したウィンドウのあるスクリーンのdpiを取得する。それ以外の場合は無視される。</param>
	/// <remarks>Per-Monitor Awareの場合、現在配置されているウィンドウのDPIを返す。</remarks>
	float GetDpiScaling(HWND hWnd = nullptr);
}