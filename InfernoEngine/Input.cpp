#include"Input.h"

//IME–³Œø‰»‚Ìˆ×‚É•K—v
#include<imm.h>
#pragma comment(lib, "imm32.lib")

namespace Inferno
{
Input::Input() :
m_LButtonPressed(false), m_mouseX(0), m_mouseY(0), m_hIMC(nullptr)
{
	for (auto& e : m_keysDown) e = false;
	for (auto& e : m_keysPressed) e = false;
}

int Input::GetMouseX() const
{
	return m_mouseX;
}

int Input::GetMouseY() const
{
	return m_mouseY;
}

bool Input::IsMouseLButtonPressed() const
{
	return m_LButtonPressed;
}

bool Input::IsKeyDown(const unsigned char vkey) const
{
	return m_keysDown[vkey];
}

bool Input::IsKeyPressed(const unsigned char vkey)
{
	bool rvalue = m_keysPressed[vkey];
	m_keysPressed[vkey] = false;
	return rvalue;
}

bool Input::IsAnyKeyPressed() const
{
	for (int i = 0; i < InputNS::KeyLen; i++)
	{
		if (m_keysDown[i] == true || m_keysPressed[i] == true)
		{
			return true;
		}
	}
	return false;

}

void Input::ClearKeys()
{
	for (int i = 0; i < InputNS::KeyLen; i++)
	{
		m_keysDown[i] = false;
		m_keysPressed[i] = false;
	}
}

void Input::ClearMouseLButton()
{
	m_LButtonDown = false;
	m_LButtonPressed = false;
}

void Input::DisableIME(HWND hWnd)
{
	m_hIMC = ImmAssociateContext(hWnd, nullptr);
}

void Input::EnableIME(HWND hWnd)
{
	if (m_hIMC)
	{
		ImmAssociateContext(hWnd, m_hIMC);
	}
}

LRESULT Input::InputProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	UNREFERENCED_PARAMETER(hWnd);

	switch (msg)
	{
	case WM_MOUSEMOVE:
		m_mouseX = LOWORD(lp);
		m_mouseY = HIWORD(lp);
		break;
	case WM_LBUTTONDOWN:
		m_LButtonDown = true;
		m_LButtonPressed = true;
		m_mouseX = LOWORD(lp);
		m_mouseY = HIWORD(lp);
		break;
	case WM_LBUTTONUP:
		m_LButtonDown = false;
		m_mouseX = LOWORD(lp);
		m_mouseY = HIWORD(lp);
		break;
	case WM_KEYDOWN:
		m_keysDown[wp] = true;
		m_keysPressed[wp] = true;
		break;
	case WM_KEYUP:
		m_keysDown[wp] = false;
		break;

	default:
		break;
	}
	return S_OK;
}

}