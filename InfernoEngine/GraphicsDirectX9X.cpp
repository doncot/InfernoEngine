#include"GraphicsDirectX9X.h"

#ifdef USE_DIRECTX9X
#include"Common.h"
#include<stdexcept>

#pragma comment(lib, "d3d9.lib" )
#pragma comment(lib, "d3dx9.lib" )

namespace Inferno
{
GraphicsDirectX9X::GraphicsDirectX9X(HWND hWnd)
:m_d3d(nullptr), m_d3dDevice(nullptr), m_sprite(nullptr)
{
	//D3Dオブジェクトを生成
	m_d3d = Direct3DCreate9(D3D_SDK_VERSION);

	//D3Dデバイスを生成するための構造体を設定する
	D3DPRESENT_PARAMETERS d3dp;
	ZeroMemory(&d3dp, sizeof(d3dp));
	d3dp.Windowed = TRUE;
	d3dp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dp.EnableAutoDepthStencil = TRUE;
	d3dp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; //V-SYNCをする

	m_d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dp,
		&m_d3dDevice
		);

	//スプライト作成
	if (FAILED(D3DXCreateSprite(m_d3dDevice, &m_sprite)))
	{
		throw std::runtime_error("スプライト作成に失敗");
	}

	//初期化が完了したら青クリアをする
	this->Clear(0, 51, 102);
}

GraphicsDirectX9X::~GraphicsDirectX9X() noexcept
{
	SAFE_RELEASE(m_sprite);
	SAFE_RELEASE(m_d3dDevice);
	SAFE_RELEASE(m_d3d);
}

void GraphicsDirectX9X::Clear(int r, int g, int b)
{
	m_d3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(r, g, b), 1.0f, 0);
	m_d3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

bool GraphicsDirectX9X::BeginScene() const
{
	m_d3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 128), 1.0f, 0);
	return SUCCEEDED(m_d3dDevice->BeginScene());
}

void GraphicsDirectX9X::EndScene() const
{
	m_d3dDevice->EndScene();
	m_d3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

bool GraphicsDirectX9X::BeginSprite() const
{
	return SUCCEEDED(m_sprite->Begin(D3DXSPRITE_ALPHABLEND));
}

void GraphicsDirectX9X::EndSprite() const
{
	m_sprite->End();
}

LPDIRECT3DDEVICE9 GraphicsDirectX9X::GetDevice() const
{
	return m_d3dDevice;
}

LPD3DXSPRITE GraphicsDirectX9X::GetSprite() const
{
	return m_sprite;
}

//bool GraphicsDirectX9X::LoadTexture(const std::wstring& filename,unsigned* width, unsigned* height, LP_TEXTURE* tex) const
//{
//	if (filename.empty())
//	{
//		tex = nullptr;
//		return false;
//	}
//
//	HRESULT hr=0;
//	D3DXIMAGE_INFO info;
//
//	hr = D3DXGetImageInfoFromFile(filename.c_str(), &info);
//	if (hr != D3D_OK)
//	{
//		throw std::logic_error(filename + std::wstring(L"が見つかりませんでした。"));
//	}
//
//	hr = D3DXCreateTextureFromFileExW(
//		m_d3dDevice,
//		filename.c_str(),
//		info.Width,info.Height,
//		1,0,
//		D3DFMT_UNKNOWN,
//		D3DPOOL_MANAGED,
//		D3DX_DEFAULT,
//		D3DX_DEFAULT,
//		0,
//		&info,
//		nullptr,
//		tex
//		);
//
//	if (hr == D3DXERR_INVALIDDATA)
//	{
//		throw FileNotFound(filename);
//	}
//
//	*width = info.Width;
//	*height = info.Height;
//
//	return true;
//}

//void GraphicsDirectX9X::TransformSprite(const Matrix* m) const
//{
//	m_sprite->SetTransform(&D3DXMATRIX(m->GetD3DMatrix()));
//}

void GraphicsDirectX9X::DrawSprite(const LP_TEXTURE tex, Vec2<float> center, const int alpha) const
{
	m_sprite->Draw(tex, nullptr, &D3DXVECTOR3(center.x, center.y, 0),
		nullptr, D3DCOLOR_ARGB(alpha, 255, 255, 255));
}

}

#endif //USE_DIRECTX9X