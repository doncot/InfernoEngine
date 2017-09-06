/*
�Q�[���p�̃E�B���h�E���N���X����������
*/
#pragma once
#include"WindowsCommon.h"

namespace Inferno
{
	///���������̔z�u
	enum class HorizontalAlignment
	{
		Left,
		Center,
		Right,
	};

	///���������̔z�u
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
	///���݂̈ʒu�ƃT�C�Y�ŃE�B���h�E���A�N�e�B�u�ɂ���
	///</summary>
	void Show() const;
	void SetTitle(LPCTSTR str) const;

	/// <summary>
	/// �N���C�A���g�̈��傫����ݒ�
	/// </summary>
	/// <param name="logicalClientWidth">DPI�X�P�[�����O���l�����Ȃ��_���I�ȕ�</param>
	/// <param name="logicalClientHeight">DPI�X�P�[�����O���l�����Ȃ��_���I�ȍ���</param>
	/// <remarks>DPI�X�P�[�����O�͓����Ōv�Z�����BWM_DPICHANGE�ŌĂяo���ۂ�DPI�͓����ł͂܂��ς���ĂȂ��̂ŁA���ӁB</remarks>
	virtual void Resize(const int logicalClientWidth, const int logicalClientHeight);

	/// <summary>
	/// �E�B���h�E�̔z�u��ݒ�
	/// </summary>
	void SetWindowAlignment(HorizontalAlignment h, VerticalAlignment v);

	static constexpr DWORD GetWindowStyle() noexcept { return WindowStyle; }

	///<summary>
	///�E�B���h�E���I��������
	///</summary>
	void Destroy();

protected:
	WNDCLASSEX m_wc;
	HWND m_hWnd; //���g���w���E�B���h�E�n���h��

	//�萔
	///�E�B���h�E�X�^�C��
	//AdjustWindowRect�ɂăo�O��������WS_CAPTION��ݒ肷��K�v������iWS_SYSMENU�Ɋ܂܂��̂ŏ璷�Ȃ͂������A�v�Z�ɓ���ĂȂ��j
	static constexpr DWORD WindowStyle =
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
	
private:
	SimpleWindow(SimpleWindow& w) = delete;
	SimpleWindow operator=(SimpleWindow& w) = delete;
};

	LRESULT WINAPI SimpleWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}