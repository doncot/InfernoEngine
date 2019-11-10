#include"GraphicsDirectX11.h"
#include"WindowsCommon.h"
#include"DpiUtility.h"
#include<DirectXTK\DDSTextureLoader.h>
#include<stdexcept>
#include<ntverp.h>
#include<Winuser.h>

using namespace std;
using namespace Inferno;
using namespace DirectX;
using namespace Microsoft::WRL;

struct COLOR_VERTEX_FVF
{
	float x, y, z;
	XMFLOAT4 color;
};

struct COLOR_TEXTURE_VERTEX_FVF
{
	float x, y, z;
	XMFLOAT4 color;
	float TexCoord[2];
};

COLOR_VERTEX_FVF s_plateVertexes[] =
{
	{ -0.25f, 0.25f, 0.0f, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ 0.25f, 0.25f, 0.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ -0.25f, -0.25f, 0.0f, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ 0.25f, -0.25f, 0.0f, XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) }
};

COLOR_TEXTURE_VERTEX_FVF s_plateTexturedVertexes[] =
{
	{ -0.25f, 0.25f, 0.0f, XMFLOAT4(0.0f, 0.0f, 1.0f, 0.8f), {0.0f, 0.0f} },
	{ 0.25f, 0.25f, 0.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 0.8f), {1.0f, 0.0f} },
	{ -0.25f, -0.25f, 0.0f, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.8f), {0.0f, 1.0f} },
	{ 0.25f, -0.25f, 0.0f, XMFLOAT4(1.0f, 0.0f, 1.0f, 0.8f), {1.0f, 1.0f} }
};

constexpr D3D11_INPUT_ELEMENT_DESC s_coloredVertexDescription[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

constexpr D3D11_INPUT_ELEMENT_DESC s_texturedVertexDescription[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};


// VS用の変換行列
struct VSConstantBuffer
{
	XMMATRIX mWorld;		//ワールド変換行列
	XMMATRIX mView;			//ビュー変換行列
	XMMATRIX mProjection;	//透視射影変換行列
};

GraphicsDirectX11::GraphicsDirectX11(HWND hWnd)
{
	//dpi
	//dpi aware
	const float dpiScale = GetDpiScaling(hWnd);

	//client size
	RECT clientSize;
	::GetClientRect(hWnd, &clientSize);
	const int scaledClientWidth = static_cast<int>(clientSize.right - clientSize.left);
	const int scaledClientHeight = static_cast<int>(clientSize.bottom - clientSize.top);

	//DirectX 3D Device
	{
		UINT createDeviceFlags = 0;
		//Direct2Dとの連携に必要らしい
		createDeviceFlags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG) && VER_PRODUCTBUILD > 9600
		//Win 8.1 SDKだと使えない
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			//D3D_FEATURE_LEVEL_10_1,
			//D3D_FEATURE_LEVEL_10_0,
		};
		D3D_FEATURE_LEVEL selectedFeatureLevel;

		ComPtr<ID3D11Device> pDevice;
		ComPtr<ID3D11DeviceContext> pDC;
		ThrowIfFailed(
			::D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
				createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
				&pDevice, &selectedFeatureLevel, &pDC)
		);

		//cast to 11.1 interface
		pDevice.As(&m_pDevice);
		pDC.As(&m_pDC);
	}

	//DXGI
	{
		// Obtain DXGI factory from device (since we used nullptr for pDxgiAdapter above)
		ThrowIfFailed(m_pDevice.As(&m_dxgiDevice));

		ComPtr<IDXGIAdapter> pDxgiAdapter = nullptr;
		ThrowIfFailed(m_dxgiDevice->GetAdapter(&pDxgiAdapter));

		ComPtr<IDXGIFactory2> pDxgiFactory = nullptr;
		ThrowIfFailed(pDxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &pDxgiFactory));
		if (pDxgiFactory == nullptr)
			throw runtime_error("failed to create IDXGIFactory");

#pragma region swap chain
		DXGI_SWAP_CHAIN_DESC1 scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC1));
		// fill the swap chain description struct
		scd.BufferCount = 2;
		//scd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
		scd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		//scd.OutputWindow = hWnd;
		scd.Scaling = DXGI_SCALING_STRETCH;
		// how many multisamples (quality of AA)
		scd.SampleDesc.Count = 4;
		// windowed/full-screen mode
		//scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL; // 描画後のバッファの扱い
		scd.Width = scaledClientWidth;
		scd.Height = scaledClientHeight;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //フルスクリーンモード切替の許可

		ThrowIfFailed(pDxgiFactory->CreateSwapChainForHwnd(m_pDevice.Get(), hWnd, &scd,
			nullptr, nullptr, &m_pSwapChain1));

		// キューに格納されていく描画コマンドをスワップ時に全てフラッシュする
		m_dxgiDevice->SetMaximumFrameLatency(1);
#pragma endregion swap chain
	}

#pragma region DirectX 2D
	{
		D2D1_FACTORY_OPTIONS d2dOpt;
		ZeroMemory(&d2dOpt, sizeof d2dOpt);
		ComPtr<ID2D1Factory1> d2dFactory;
		auto hr = D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory1),
			&d2dOpt,
			reinterpret_cast<void**>(d2dFactory.GetAddressOf()));
		if (hr == E_NOINTERFACE)
			throw runtime_error("D2D1CreateFactory() : アップデートが必要です");
		if (FAILED(hr))
			throw runtime_error("D2D1CreateFactory()");

		// Direct2Dデバイスの作成
		hr = d2dFactory->CreateDevice(m_dxgiDevice.Get(), &m_p2dDevice);
		if (FAILED(hr))
			throw runtime_error("ID2DFactory#CreateDevice()");

		// Direct2Dデバイスコンテクストの作成
		hr = m_p2dDevice->CreateDeviceContext(
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			&m_p2dDC);
		if (FAILED(hr))
			throw runtime_error("ID2DFactory#CreateDeviceContext()");
	
		// レンダーターゲットの取得（DXGI）
		ComPtr<IDXGISurface1> pDxgiSurface;
		hr = m_pSwapChain1->GetBuffer(0, IID_PPV_ARGS(&pDxgiSurface));
		if (FAILED(hr))
			throw runtime_error("IDXGISwapChain1#GetBuffer()");

		// Direct2Dの描画先となるビットマップを作成
		D2D1_BITMAP_PROPERTIES1 d2dProp =
			D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
				0, 0);
		hr = m_p2dDC->CreateBitmapFromDxgiSurface(
			pDxgiSurface.Get(), &d2dProp, &m_2dTargetBitmap);
		if (FAILED(hr))
			throw runtime_error("ID2D1DeviceContext#CreateBitmapFromDxgiSurface()");

		// 描画するDirect2Dビットマップの設定
		m_p2dDC->SetTarget(m_2dTargetBitmap.Get());
	}
#pragma endregion DirectX 2D

#pragma region DirectWrite
	{
		ThrowIfFailed(DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory1),
			reinterpret_cast<IUnknown**>(m_pWriteFactory.GetAddressOf())
		));

		float const FontSize = 64.0f;
		ThrowIfFailed(m_pWriteFactory->CreateTextFormat(
			TEXT("Consolas"),                // Font family name.
			nullptr,                       // Font collection (NULL sets it to use the system font collection).
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			FontSize * dpiScale,
			TEXT("ja-jp"),
			&m_pTextFormat
		));

		//alignment
		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);

		//D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		m_layoutRect = D2D1::RectF(
			static_cast<float>(clientSize.left),
			static_cast<float>(clientSize.top),
			static_cast<float>(clientSize.right - clientSize.left),
			static_cast<float>(clientSize.bottom - clientSize.top)
		);
	}
#pragma endregion DirectWrite

	//DirectX 3D

	// get the address of the back buffer
	ComPtr<ID3D11Texture2D> pBackBuffer;
	m_pSwapChain1->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

	// use the back buffer address to create the render target
	m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pBackBuffer);

	// set the render target as the back buffer
	//GetAddressOfを使う理由：http://gamedev.stackexchange.com/questions/81802/how-do-i-use-com-ptr-t-with-rendertargetview-and-depthstencilview
	m_pDC->OMSetRenderTargets(1, m_pBackBuffer.GetAddressOf(), nullptr);

	//***********
	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)scaledClientWidth;
	viewport.Height = (float)scaledClientHeight;

	m_pDC->RSSetViewports(1, &viewport);

	//************
	//shader
	ComPtr<ID3DBlob> VS, PS, PS2;
	D3DCompileFromFile(TEXT("shader.hlsl"), nullptr, nullptr, "VShader", "vs_5_0", 0, 0, &VS, nullptr);
	D3DCompileFromFile(TEXT("shader.hlsl"), nullptr, nullptr, "PShader", "ps_5_0", 0, 0, &PS, nullptr);
	D3DCompileFromFile(TEXT("PS_InvertColor.hlsl"), nullptr, nullptr, "main", "ps_5_0", 0, 0, &PS2, nullptr);

	m_pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &m_pVS);
	m_pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &m_pPS);
	m_pDevice->CreatePixelShader(PS2->GetBufferPointer(), PS2->GetBufferSize(), nullptr, &m_pPS_InvertColor);

#pragma region VertexBuffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(COLOR_TEXTURE_VERTEX_FVF) * 4;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
	m_pDevice->CreateBuffer(&bd, nullptr, &m_pPlateVertexBuffer);       // create the buffer

	D3D11_MAPPED_SUBRESOURCE ms;
	m_pDC->Map(m_pPlateVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	//GPUに頂点データを転送
	memcpy(ms.pData, s_plateTexturedVertexes, sizeof(s_plateTexturedVertexes));
	m_pDC->Unmap(m_pPlateVertexBuffer.Get(), 0);

	//Input assemblerステージで頂点データがどのように配置されるを通知
	ComPtr<ID3D11InputLayout> pLayout;
	m_pDevice->CreateInputLayout(s_texturedVertexDescription, ARRAYSIZE(s_texturedVertexDescription),
		VS->GetBufferPointer(), VS->GetBufferSize(), pLayout.GetAddressOf());
	m_pDC->IASetInputLayout(pLayout.Get());
#pragma endregion //VertexBuffer

#pragma region vs constbuffer
	// Fill in a buffer description.
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(VSConstantBuffer);
	////map/unmapを使う場合
	//cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	//cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//UpdateSubresourceを使う場合
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Fill in the subresource data.
	//D3D11_SUBRESOURCE_DATA InitData;
	//InitData.pSysMem = &vsConstBuff;
	//InitData.SysMemPitch = 0;
	//InitData.SysMemSlicePitch = 0;

	//m_pDevice->CreateBuffer(&cbDesc, &InitData, &m_pVSConstantBuffer);
	m_pDevice->CreateBuffer(&cbDesc, nullptr, &m_pVSConstantBuffer);

	m_pDC->VSSetConstantBuffers(0, 1, m_pVSConstantBuffer.GetAddressOf());
#pragma endregion //vs const buffer

#pragma region Texture
	ComPtr<ID3D11ShaderResourceView> pSRV = nullptr;
	auto result = CreateDDSTextureFromFile(m_pDevice.Get(),
		TEXT("eagle.dds"),
		nullptr, pSRV.GetAddressOf());
	if (FAILED(result))
		throw runtime_error("cannot load texture.");

	//そのテクスチャーをコンテキストに設定
	ID3D11ShaderResourceView* hpShaderResourceViews[] = { pSRV.Get() };
	m_pDC->PSSetShaderResources(0, 1, hpShaderResourceViews);

#pragma endregion //Texture

#pragma region blending
	ComPtr<ID3D11BlendState1> pBlendState = nullptr;
	D3D11_BLEND_DESC1 blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC1));
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ThrowIfFailed(m_pDevice->CreateBlendState1(&blendDesc, &pBlendState));

	m_pDC->OMSetBlendState(pBlendState.Get(), 0, 0xffffffff);
#pragma endregion //blending

#ifdef _DEBUG
	//debug
	m_pDevice.As(&m_pD3dDebug);
#endif //_DEBUG

	//透視射影変換行列
	XMStoreFloat4x4(& m_projection, 
		XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 
						static_cast<float>(scaledClientWidth) / static_cast<float>(scaledClientHeight),
						0.1f, 1000.0f)
	);

}

GraphicsDirectX11::~GraphicsDirectX11()
{
	m_pDC->ClearState();

	//m_pD3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
}

/// <summary>
/// 指定色でクリア
/// </summary>
void GraphicsDirectX11::SetBackgroundColor(int r, int g, int b, int a)
{
	XMFLOAT4 color;

	color.x = r / 255.0f;
	color.y = g / 255.0f;
	color.z = b / 255.0f;
	color.w = a / 255.0f;

	m_backgroundColor = std::move(color);
}

void GraphicsDirectX11::SetBackgroundColor(float r, float g, float b, float a)
{
	m_backgroundColor = XMFLOAT4(r, g, b, a);
}

void GraphicsDirectX11::LookAt(const XMVECTOR& eye, const XMVECTOR& at)
{
	//XMMATRIX hView;		//ビュー変換行列
	//XMVECTOR hEye = XMVectorSet(0.0, 0.0f, -1.75f, 0.0f);	//カメラの位置
	//XMVECTOR hAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);		//焦点の位置
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	
	XMStoreFloat4x4(&m_view, XMMatrixLookAtLH(eye, at, up));
}  

void GraphicsDirectX11::Draw()
{
	m_pDC->ClearRenderTargetView(m_pBackBuffer.Get(), &m_backgroundColor.x);

	/*
	DirectX rendering Pipeline prefixes
	Input Assembler Stage	IA***
	Vertex Shader Stage	VS***
	Hull Shader	HS***
	Domain Shader Stage	DS***
	Geometry Shader Stage	GS***
	Stream Output Stage	SO***
	Rasterizer Stage	RS***
	Pixel Shader Stage	PS***
	Output Merger Stage	OM***
	*/

#pragma region 2d
	if (m_p2dDC)
	{
		const D2D1_SIZE_F targetSize = m_p2dDC->GetSize();

		D2D1_COLOR_F color;
		color = D2D1::ColorF(
			1.0f, 1.0f, 1.0f,
			1.0f);

		ComPtr<ID2D1SolidColorBrush> pBrush;
		auto hr = m_p2dDC->CreateSolidColorBrush(color, &pBrush);
		if (FAILED(hr))
			throw runtime_error("ID2D1DeviceContext#CreateSolidColorBrush()");

		m_p2dDC->BeginDraw();
		{
			//m_pRenderTarget->SetTransform(D2D1::IdentityMatrix());
			//m_pBackBufferRenderTarget->SetTransform(
			//	D2D1::Matrix3x2F::Scale(targetSize)
			//);
			m_p2dDC->SetTransform(
				D2D1::IdentityMatrix()
			);

			//m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

			//LPCTSTR text = _T("ようこそ DirectWrite!!");
			m_p2dDC->DrawTextW(
				Text.c_str(),        // The string to render.
				_tcsclen(Text.c_str()),    // The string's length.
				m_pTextFormat.Get(),    // The text format.
				D2D1::RectF(0, 0, targetSize.width, targetSize.height),
				pBrush.Get()     // The brush used to draw the text.
			);
		}
		m_p2dDC->EndDraw();
	}
#pragma endregion //2d

	// select which vertex buffer to display
	UINT stride = sizeof(COLOR_TEXTURE_VERTEX_FVF);
	UINT offset = 0;
	m_pDC->IASetVertexBuffers(0, 1, m_pPlateVertexBuffer.GetAddressOf(), &stride, &offset);

	// select which primtive type we are using
	m_pDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//shader
	m_pDC->VSSetShader(m_pVS.Get(), nullptr, 0);

	if (!Invert)
		m_pDC->PSSetShader(m_pPS.Get(), nullptr, 0);
	else
		m_pDC->PSSetShader(m_pPS_InvertColor.Get(), nullptr, 0);

#pragma region transform
	XMMATRIX hWorld;		//ワールド変換行列
	hWorld = XMMatrixIdentity();
	static float debug = 0.0f;
	debug -= 0.001f;
	hWorld *= XMMatrixRotationZ(debug);

	VSConstantBuffer vsConstBuff;
	vsConstBuff.mWorld = XMMatrixTranspose(hWorld);
	vsConstBuff.mView = XMMatrixTranspose(XMLoadFloat4x4(&m_view));
	vsConstBuff.mProjection = XMMatrixTranspose(XMLoadFloat4x4(&m_projection));

	//更新
	m_pDC->UpdateSubresource(m_pVSConstantBuffer.Get(), 0, nullptr, &vsConstBuff, 0, 0);
#pragma endregion //transform

	// draw the vertex buffer to the back buffer
	m_pDC->Draw(4, 0);

#pragma region transform
	hWorld = XMMatrixIdentity();
	static float debug2 = 0.0f;
	debug2 += 0.005f;
	hWorld *= XMMatrixScaling(0.7f, 0.7f, 1.0f);
	hWorld *= XMMatrixRotationZ(debug2);
	hWorld *= XMMatrixTranslation(0.2f, 0.0f, 0.01f);

	vsConstBuff.mWorld = XMMatrixTranspose(hWorld);
	vsConstBuff.mView = XMMatrixTranspose(XMLoadFloat4x4(&m_view));
	vsConstBuff.mProjection = XMMatrixTranspose(XMLoadFloat4x4(&m_projection));

	//更新
	m_pDC->UpdateSubresource(m_pVSConstantBuffer.Get(), 0, nullptr, &vsConstBuff, 0, 0);
#pragma endregion //transform

	// draw the vertex buffer to the back buffer
	m_pDC->Draw(4, 0);

	// switch the back buffer and the front buffer
	m_pSwapChain1->Present(0, 0);
	
}

void GraphicsDirectX11::RecreateScreen(HWND hWnd, float dpiScaling)
{
	if (m_pSwapChain1 != nullptr &&
		m_pDevice != nullptr &&
		m_pDC != nullptr &&
		m_pBackBuffer != nullptr)
	{
		//現在の実際のクライアント領域（物理ピクセル）で取得
		RECT clientSize = {0};
		::GetClientRect(hWnd, &clientSize);
		const int clientWidth = clientSize.right - clientSize.left;
		const int clientHeight = clientSize.bottom - clientSize.top;

		//注意：IDXGISwapChain::ResizeBuffersを呼ぶ前に、繋がってる全てのリソースを解除する

		//2d
		m_p2dDC->SetTarget(nullptr);
		m_2dTargetBitmap.Reset();

		//3d:レンダリングターゲットを解除
		m_pDC->OMSetRenderTargets(0, nullptr, nullptr);
		// Release all outstanding references to the swap chain's buffers.
		m_pBackBuffer.Reset();

		HRESULT hr;
		// Preserve the existing buffer count and format.
		// Automatically choose the width and height to match the client rect for HWNDs.
		hr = m_pSwapChain1->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		if (FAILED(hr)) throw runtime_error("failed to resize buffers");

#pragma region Direct3D
		// Get buffer and create a render-target-view.
		ComPtr<ID3D11Texture2D> pBuffer;
		hr = m_pSwapChain1->GetBuffer(0, __uuidof(ID3D11Texture2D),
			(void**)&pBuffer);
		if (FAILED(hr)) throw runtime_error("failed to create render buffer");

		hr = m_pDevice->CreateRenderTargetView(pBuffer.Get(), nullptr,
			&m_pBackBuffer);
		if (FAILED(hr)) throw runtime_error("failed to create render target");

		m_pDC->OMSetRenderTargets(1, m_pBackBuffer.GetAddressOf(), nullptr);

		// Set up the viewport.
		D3D11_VIEWPORT vp;
		ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
		vp.Width = (float)clientWidth;
		vp.Height = (float)clientHeight;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_pDC->RSSetViewports(1, &vp);
#pragma endregion Direct3D

#pragma region Direct2D
		// レンダーターゲットの取得（DXGI）
		ComPtr<IDXGISurface1> pDxgiSurface;
		hr = m_pSwapChain1->GetBuffer(0, IID_PPV_ARGS(&pDxgiSurface));
		if (FAILED(hr))
			throw runtime_error("IDXGISwapChain1#GetBuffer()");

		// Direct2Dの描画先となるビットマップを作成
		D2D1_BITMAP_PROPERTIES1 d2dProp =
			D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
				(float)clientWidth, (float)clientHeight);
		hr = m_p2dDC->CreateBitmapFromDxgiSurface(
			pDxgiSurface.Get(), &d2dProp, &m_2dTargetBitmap);

		// 描画するDirect2Dビットマップの設定
		m_p2dDC->SetTarget(m_2dTargetBitmap.Get());

#pragma endregion Direct2D

#pragma region DirectWrite
		float const FontSize = 64.0f;
		ThrowIfFailed(m_pWriteFactory->CreateTextFormat(
			TEXT("Consolas"),                // Font family name.
			nullptr,                       // Font collection (NULL sets it to use the system font collection).
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			FontSize * dpiScaling,
			TEXT("ja-jp"),
			&m_pTextFormat
		));

		//alignment
		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);

		m_layoutRect = D2D1::RectF(
			static_cast<float>(clientSize.left),
			static_cast<float>(clientSize.top),
			static_cast<float>(clientSize.right - clientSize.left),
			static_cast<float>(clientSize.bottom - clientSize.top)
		);
#pragma endregion DirectWrite
	}
}

void GraphicsDirectX11::SetFullScreenState(bool isFullscreen)
{
	m_pSwapChain1->SetFullscreenState(isFullscreen, nullptr);
}
