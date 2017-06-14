#pragma once
#include<Windows.h>

namespace InputNS {
	const int KeyLen = 256;
}

namespace Inferno
{
class Input
{
public:
	Input();

	int GetMouseX() const;
	int GetMouseY() const;
	bool IsMouseLButtonPressed() const;
	bool IsKeyDown(const unsigned char vkey) const;
	//IskeyPressedが真になった場合、呼び出し後該当キーはリセットされる（リセットしないバージョンも需要あれば作る予定）。
	bool IsKeyPressed(const unsigned char vkey);
	bool IsAnyKeyPressed() const;

	void ClearMouseLButton();
	void ClearKeys();

	void DisableIME(HWND hWnd);
	void EnableIME(HWND hWnd);

	LRESULT InputProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

private:
	//マウス関連
	bool m_LButtonDown;
	bool m_LButtonPressed;
	int m_mouseX;
	int m_mouseY;
	//キーボード関連
	bool m_keysDown[InputNS::KeyLen];
	bool m_keysPressed[InputNS::KeyLen];

	//入力コンテキストハンドル（IMEを無効にする際保存しておき、再び有効に戻す際に使う）
	HIMC m_hIMC;
};



}