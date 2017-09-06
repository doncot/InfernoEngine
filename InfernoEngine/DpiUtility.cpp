#include "DpiUtility.h"
#include <stdexcept>
#include <ShellScalingApi.h>
#pragma comment(lib, "Shcore.lib")

float Inferno::GetDpi(HWND hWnd)
{
	PROCESS_DPI_AWARENESS  dpiAwareness;
	HRESULT hr = GetProcessDpiAwareness(nullptr, &dpiAwareness);
	if (hr != S_OK)
	{
		throw std::runtime_error("Failed to GetProcessDpiAwareness");
	}

	UINT dpi = 0;
	switch (dpiAwareness)
	{
	case PROCESS_DPI_AWARENESS::PROCESS_SYSTEM_DPI_AWARE:
		dpi = ::GetDpiForSystem();

		break;

	case PROCESS_DPI_AWARENESS::PROCESS_PER_MONITOR_DPI_AWARE:
	{
		// per-monitor aware
		// モニターのハンドルを取得
		const HMONITOR hMonitor = ::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
		unsigned int dpiY;
		// モニターのdpiを取得
		::GetDpiForMonitor(hMonitor, MONITOR_DPI_TYPE::MDT_EFFECTIVE_DPI, &dpi, &dpiY);
	}
	break;

	default:
		throw std::logic_error("unknown dpi awareness");
	}

	return dpi;
}

float Inferno::GetDpiScaling(HWND hWnd)
{
	return GetDpi(hWnd) / static_cast<float>(USER_DEFAULT_SCREEN_DPI);
}
