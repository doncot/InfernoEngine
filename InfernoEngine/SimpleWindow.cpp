#include"SimpleWindow.h"
#include"DpiUtility.h"
#include"Shellapi.h"
#include<WinUser.h>

//���������[�N���o�p�B
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

	//�v���V�[�W���ƒʐM�ł���悤�Ɏ������g�����̃X�R�[�v�Ŋo��������
	s_pGame = this;

	//WindowClass�ݒ�
	m_wc.cbSize = sizeof(WNDCLASSEX);
	//CS_CLASSDC:�S�ẴE�B���h�E�������f�o�C�X�R���e�L�X�g�����L����e
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

	//WindowClass�o�^
	if (!RegisterClassEx(&m_wc))
	{
		throw std::runtime_error("�E�B���h�E�N���X�̓o�^�Ɏ��s���܂����B");
	}

	//�E�B���h�E�𐶐�
	m_hWnd = CreateWindow(className, TEXT("SimpleWindow"),
		WindowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, //�E�B���h�E�ʒu
		CW_USEDEFAULT, CW_USEDEFAULT, //�E�B���h�E�T�C�Y
		nullptr, nullptr, m_wc.hInstance, nullptr);
	if (m_hWnd == nullptr)
	{
		::UnregisterClass(className, m_wc.hInstance);
		throw std::runtime_error("�E�B���h�E�̐����Ɏ��s���܂����B");
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
	//dpi�v�Z
	const auto dpiScaling = GetDpiScaling();

	const int scaledClientWidth = static_cast<int>(logicalClientWidth * dpiScaling);
	const int scaledClientHeight = static_cast<int>(logicalClientHeight * dpiScaling);

	RECT newWindowSize = {0, 0, scaledClientWidth, scaledClientHeight };
	::AdjustWindowRect(&newWindowSize, WindowStyle, FALSE);

	const int windowWidth = newWindowSize.right - newWindowSize.left;
	const int windowHeight = newWindowSize.bottom - newWindowSize.top;

	// �E�C���h�E�T�C�Y�̍Đݒ�
	SetWindowPos(m_hWnd, HWND_TOP, 0, 0, windowWidth, windowHeight, SWP_NOMOVE);
}

void SimpleWindow::SetWindowAlignment(HorizontalAlignment h, VerticalAlignment v)
{
	//GetWindowPlacement��SetWindowPlacement��
	//�ʒu�̕ۑ��ƕ������ł���

	// �E�C���h�E�S�̂̉����̕����v�Z
	RECT rect;
	::GetWindowRect(m_hWnd, &rect);		// �E�C���h�E�S�̂̃T�C�Y�擾
	const int ww = rect.right - rect.left;	// �E�C���h�E�S�̂̕��̉������v�Z
	const int wh = rect.bottom - rect.top;	// �E�C���h�E�S�̂̕��̏c�����v�Z

	::GetClientRect(m_hWnd, &rect);
	const int cw = rect.right - rect.left;
	const int ch = rect.bottom - rect.top;

	//result
	int rx = 0;
	int ry = 0;

	//�ʒu�����W�ɒ���
	switch(h)
	{
	case HorizontalAlignment::Center:
		rx = (GetSystemMetrics(SM_CXSCREEN) - ww) / 2;
		if (rx < 0) rx=0; //��ʂ������������ꍇ
		break;
	default:
		assert(false);
		break;
	}

	switch(v)
	{
	case VerticalAlignment::Center:
		ry = (GetSystemMetrics(SM_CYSCREEN) - wh) / 2;
		if (ry < 0) ry = 0; //��ʂ������������ꍇ
		break;
	default:
		assert(false);
		break;
	}

	//adjust (offset)
	int ax = 0;
	int ay = 0;

	//�^�X�N�o�[���������ꍇ��������
	//�^�X�N�o�[�̈ʒu����`�œ���
	APPBARDATA sBarInfo;
	ZeroMemory(&sBarInfo, sizeof(APPBARDATA));
	sBarInfo.cbSize = sizeof(APPBARDATA);
	sBarInfo.hWnd = m_hWnd;
	SHAppBarMessage(ABM_GETTASKBARPOS, &sBarInfo);
	RECT r = sBarInfo.rc;
	//�^�X�N�o�[�����ɂ��������̑Ώ�
	if (r.left <= 0 && r.top <= 0 &&//�^�X�N�o�[�̍��[�Ə�[��0�ȉ���
		r.right <= r.bottom) //�E�ӂ�艺�ӂ������ꍇ
	{
		ax = r.right; //�^�X�N�o�[�������̈�����炷
	}
	//�����f�X�N�g�b�v���I�[�o�[�����ꍇ��
	if (rx + ax + ww > GetSystemMetrics(SM_CXSCREEN))
	{
		//�^�X�N�o�[�A�������̓f�X�N�g�b�v�[�ɃW���X�g�����悤�ɂ���
		rx = ax - (ww - cw) / 2;  //���̒l�̓E�B���h�E�g�����Y�����Ă���
		ax = 0;
	}

	//�^�X�N�o�[����ɂ��������̑Ώ�
	if (r.left <= 0 && r.top <= 0 &&//�^�X�N�o�[�̍��[�Ə�[��0�ȉ���
		r.bottom <= r.right) //���ӂ��E�ӂ������ꍇ
	{
		ay = r.bottom;//�^�X�N�o�[�������̈�����炷
	}
	//�����f�X�N�g�b�v���I�[�o�[�����ꍇ��
	if (ry + ay + wh > GetSystemMetrics(SM_CYSCREEN))
	{
		ry = ay - (wh - ch) / 2; //�^�X�N�o�[�A�������̓f�X�N�g�b�v�[�ɃW���X�g�����悤�ɂ���
		ay = 0;
	}
	
	::SetWindowPos(m_hWnd, HWND_TOP, rx+ax, ry+ay, 0, 0, SWP_NOSIZE);
}

void SimpleWindow::Show() const
{
	// http://stackoverflow.com/questions/17488826/the-meaning-of-sw-show-and-sw-shownormal
	::ShowWindow(m_hWnd, SW_SHOWNORMAL);

	::ValidateRect(m_hWnd, nullptr); //WM_PAINT���b�Z�[�W���o����
	::UpdateWindow(m_hWnd); //�̈���X�V
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
