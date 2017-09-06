#pragma once
#include "WindowsCommon.h"

namespace Inferno {

	/// <summary>
	/// DPI���擾����
	/// </summary>
	/// <param name="hWnd">Per-Monitor Aware�̏ꍇ�A�n�����E�B���h�E�̂���X�N���[����dpi���擾����B����ȊO�̏ꍇ�͖��������B</param>
	/// <remarks>Per-Monitor Aware�̏ꍇ�A���ݔz�u����Ă���E�B���h�E��DPI��Ԃ��B</remarks>
	float GetDpi(HWND hWnd = nullptr);

	/// <summary>
	/// DPI�X�P�[�����O���擾����
	/// </summary>
	/// <param name="hWnd">Per-Monitor Aware�̏ꍇ�A�n�����E�B���h�E�̂���X�N���[����dpi���擾����B����ȊO�̏ꍇ�͖��������B</param>
	/// <remarks>Per-Monitor Aware�̏ꍇ�A���ݔz�u����Ă���E�B���h�E��DPI��Ԃ��B</remarks>
	float GetDpiScaling(HWND hWnd = nullptr);
}