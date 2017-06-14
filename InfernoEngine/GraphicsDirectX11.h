#pragma once
#include"Graphics.h"

//#include<dxgi.h>
#include<dxgi1_2.h>
//#include<d3d11.h>
#include<d3d11_1.h>
#include<DirectXMath.h>
//#include<dwrite.h>
#include<dwrite_1.h>
//#include<D2d1.h>
#include<D2d1_1.h>
//#include<DirectXPackedVector.h>
//#include<DirectXColors.h>
#include<D3Dcompiler.h>
#include<wrl/module.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "Dwrite.lib")
#pragma comment(lib, "D2d1.lib")
#include<string>

namespace Inferno
{
	class GraphicsDirectX11 : public IGraphics
	{
	public:
		GraphicsDirectX11() = delete;

		/// <summary>
		///	コンストラクタ（RAII）
		/// </summary>
		/// <exception cref="std::runtime_error" />
		GraphicsDirectX11(HWND hWnd);
		~GraphicsDirectX11() noexcept override final;

		/// <summary>
		/// 指定色でクリア
		/// </summary>
		void SetBackgroundColor(int r, int g, int b, int a = 255) override final;
		void SetBackgroundColor(float r, float g, float b, float a = 1.0f) override final;

		void LookAt(const DirectX::XMVECTOR& eye, const DirectX::XMVECTOR& at);

		void Draw() override final;

		/// <summary>
		/// スクリーンバッファ（レンダリングターゲット）を再構成する。
		/// </summary>
		/// <exception cref="std::runtime_error">要求が無効だった場合に投げられる。</exception>
		void RecreateScreen(HWND hWnd, float dpiScaling);

		/// <summary>
		/// フルスクリーンモードの切り替え
		/// </summary>
		/// <param name="modeFlag">trueでフルスクリーン、falseでウィンドウ。</param>
		void SetFullScreenState(bool isFullscreen);

#pragma region fordebug
		bool Invert = false;
		
		std::wstring Text;
#pragma endregion fordebug
	private:
		//DXGI
		Microsoft::WRL::ComPtr<IDXGIDevice2> m_dxgiDevice;
		Microsoft::WRL::ComPtr<IDXGISwapChain1> m_pSwapChain1;

		//Windows Imaging Component Objects
		//Microsoft::WRL::ComPtr<IWICImagingFactory2> m_wicFactory;

		//D2D
		Microsoft::WRL::ComPtr<ID2D1Device> m_p2dDevice;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_p2dDC;
		Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_2dTargetBitmap;

		//DirectWrite
		Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTextFormat;
		Microsoft::WRL::ComPtr<IDWriteFactory1> m_pWriteFactory;
		D2D1_RECT_F m_layoutRect;

		//D3D
		//デバイス
		Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
		Microsoft::WRL::ComPtr<ID3D11Device1> m_pDevice1;

		//デバイスコンテキスト
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDC;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_pDC1;
		//デバッグデバイス
		Microsoft::WRL::ComPtr<ID3D11Debug> m_pD3dDebug;

		/// <summary>
		/// レンダリングターゲット（バックバッファ）
		/// </summary>
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pBackBuffer;

		//シェーダー
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVS;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPS;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPS_InvertColor;

		/// <summary>
		/// 板ポリの頂点バッファ
		/// </summary>
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pPlateVertexBuffer;

		/// <summary>
		/// VSのConstant Buffer
		/// </summary>
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVSConstantBuffer;

		DirectX::XMFLOAT4 m_backgroundColor;

		//view matrix
		DirectX::XMFLOAT4X4 m_view;
		DirectX::XMFLOAT4X4 m_projection;
	};


}