/*
ゲーム用のウィンドウをクラス化したもの
*/
#pragma once
#include"WindowsCommon.h"

namespace Inferno
{
	///水平方向の配置
	enum class HorizontalAlignment
	{
		Left,
		Center,
		Right,
	};

	///垂直方向の配置
	enum class VerticalAlignment
	{
		Top,
		Center,
		Bottom,
	};

class SimpleWindow
{
public:
	SimpleWindow();
	virtual ~SimpleWindow() noexcept;

	///<summary>
	///現在の位置とサイズでウィンドウをアクティブにする
	///</summary>
	void Show() const;
	void SetTitle(LPCTSTR str) const;

	/// <summary>
	/// クライアント領域を大きさを設定
	/// </summary>
	/// <param name="logicalClientWidth">DPIスケーリングを考慮しない論理的な幅</param>
	/// <param name="logicalClientHeight">DPIスケーリングを考慮しない論理的な高さ</param>
	/// <remarks>DPIスケーリングは内部で計算される。WM_DPICHANGEで呼び出す際はDPIは内部ではまだ変わってないので、注意。</remarks>
	virtual void Resize(const int logicalClientWidth, const int logicalClientHeight);

	/// <summary>
	/// ウィンドウの配置を設定
	/// </summary>
	void SetWindowAlignment(HorizontalAlignment h, VerticalAlignment v);

	static constexpr DWORD GetWindowStyle() noexcept { return WindowStyle; }

	///<summary>
	///ウィンドウを終了させる
	///</summary>
	void Destroy();

protected:
	WNDCLASSEX m_wc;
	HWND m_hWnd; //自身を指すウィンドウハンドル

	//定数
	///ウィンドウスタイル
	//AdjustWindowRectにてバグがあってWS_CAPTIONを設定する必要がある（WS_SYSMENUに含まれるので冗長なはずだが、計算に入れてない）
	static constexpr DWORD WindowStyle =
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
	
private:
	SimpleWindow(SimpleWindow& w) = delete;
	SimpleWindow operator=(SimpleWindow& w) = delete;
};

	LRESULT WINAPI SimpleWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}