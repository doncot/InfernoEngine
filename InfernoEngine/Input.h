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
	//IskeyPressed���^�ɂȂ����ꍇ�A�Ăяo����Y���L�[�̓��Z�b�g�����i���Z�b�g���Ȃ��o�[�W���������v����΍��\��j�B
	bool IsKeyPressed(const unsigned char vkey);
	bool IsAnyKeyPressed() const;

	void ClearMouseLButton();
	void ClearKeys();

	void DisableIME(HWND hWnd);
	void EnableIME(HWND hWnd);

	LRESULT InputProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

private:
	//�}�E�X�֘A
	bool m_LButtonDown;
	bool m_LButtonPressed;
	int m_mouseX;
	int m_mouseY;
	//�L�[�{�[�h�֘A
	bool m_keysDown[InputNS::KeyLen];
	bool m_keysPressed[InputNS::KeyLen];

	//���̓R���e�L�X�g�n���h���iIME�𖳌��ɂ���ەۑ����Ă����A�ĂїL���ɖ߂��ۂɎg���j
	HIMC m_hIMC;
};



}