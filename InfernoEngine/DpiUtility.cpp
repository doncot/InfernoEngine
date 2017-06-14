#include "DpiUtility.h"
#include <stdexcept>
#include <ShellScalingApi.h>
#pragma comment(lib, "Shcore.lib")

float Inferno::GetDpiScaling(HWND hWnd)
{
	PROCESS_DPI_AWARENESS  dpiAwareness;
	HRESULT hr = GetProcessDpiAwareness(nullptr, &dpiAwareness);
	if (hr != S_OK)
	{
		throw std::runtime_error("Failed to GetProcessDpiAwareness");
	}

	float dpiScaling = 0.0f;
	switch (dpiAwareness)
	{
	case PROCESS_DPI_AWARENESS::PROCESS_SYSTEM_DPI_AWARE:
		dpiScaling = ::GetDpiForSystem() / static_cast<float>(USER_DEFAULT_SCREEN_DPI);

		break;

	case PROCESS_DPI_AWARENESS::PROCESS_PER_MONITOR_DPI_AWARE:
	{
		// per-monitor aware
		// モニターのハンドルを取得
		const HMONITOR hMonitor = ::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
		unsigned int dpiX, dpiY;
		// モニターのdpiを取得
		::GetDpiForMonitor(hMonitor, MONITOR_DPI_TYPE::MDT_EFFECTIVE_DPI, &dpiX, &dpiY);

		const auto dpiScaleX = dpiX / static_cast<float>(USER_DEFAULT_SCREEN_DPI);
		//const double dpiScaleY = dpiY / static_cast<double>(USER_DEFAULT_SCREEN_DPI);
		dpiScaling = dpiScaleX;
	}
	break;
	default:
		throw std::logic_error("unknown dpi awareness");
	}
	return dpiScaling;
}
