/*
 グラフィックスのインクルード
*/

#pragma once
#include<memory>
#include<cassert>

namespace Inferno
{
//#define USE_DIRECTX9X

	/// <summary>
	/// 利用するグラフィックAPIの種類
	/// </summary>
	enum class GraphicApi
	{
		DirectX9X,
		DirectX10
	};

	/// <summary>
	/// グラフィックスのインターフェース
	/// </summary>
	class IGraphics
	{
	public:
		virtual ~IGraphics() noexcept = 0 {}

		/// <summary>
		/// 指定色でクリア
		/// </summary>
		virtual void SetBackgroundColor(int r, int g, int b, int a = 255) = 0;
		virtual void SetBackgroundColor(float r, float g, float b, float a = 1.0f) = 0;

		virtual void Draw() = 0;
	};

	///// <summary>
	///// グラフィックスのファクトリー
	///// </summary>
	//class Graphics
	//{
	//public:
	//	/// <summary>
	//	/// グラフィックスを生成する。
	//	/// </summary>
	//	/// <param name="apiType">グラフィクスの種類</param>
	//	/// <param name="hWnd">ウィンドウハンドル</param>
	//	/// <returns>グラフィクスインスタンスのインターフェース</returns>
	//	static std::shared_ptr<IGraphics> Create(GraphicApi apiType, HWND hWnd);
	//};
}

#include"GraphicsDirectX11.h"