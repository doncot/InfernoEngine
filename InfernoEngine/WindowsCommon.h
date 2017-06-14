/*
共通ヘッダー
win32用
*/
#pragma once

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<tchar.h>
#include<cassert>
#include<stdexcept>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p!=nullptr) { delete (p);     (p)=nullptr; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p!=nullptr) { delete[] (p);   (p)=nullptr; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=nullptr; } }
#endif

#ifndef ERROR_MESSAGE
#define ERROR_MESSAGE(x) { MessageBox(nullptr,(x),TEXT("エラー"),MB_OK|MB_ICONERROR); }
#endif

#ifndef ERROR_EXIT
#define ERROR_EXIT(x) {	 const int msg_size=_MAX_FNAME + _MAX_EXT +512;\
	TCHAR msg[msg_size];\
	TCHAR drive[_MAX_DRIVE];\
	TCHAR dir[_MAX_DIR];\
	TCHAR fname[_MAX_FNAME];\
	TCHAR ext[_MAX_EXT];\
	_tsplitpath_s(TEXT(__FILE__),drive,_MAX_DRIVE,dir,_MAX_DIR,fname,_MAX_FNAME,ext,_MAX_EXT);\
	_stprintf_s(msg, msg_size, TEXT("%s\r\nファイル：%s%s\r\n行番号：%d"),x,fname,ext,__LINE__);\
	MessageBox(nullptr,msg,TEXT("Error"),MB_OK|MB_ICONEXCLAMATION);\
	PostQuitMessage(1);\
}
#endif

// Helper class for COM exceptions
class com_exception : public std::exception
{
public:
	com_exception(HRESULT hr) : result(hr) {}

	virtual const char* what() const override
	{
		static char s_str[64] = {};
		sprintf_s(s_str, "Failure with HRESULT of %08X", result);
		return s_str;
	}

private:
	HRESULT result;
};

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw com_exception(hr);
	}
}

