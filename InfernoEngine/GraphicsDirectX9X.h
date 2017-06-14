/*
グラフィックに必要なリソースを管理する
DirectXに関連するものは全部これにまとめる
*/
#pragma once
#include"Graphics.h"

#ifdef USE_DIRECTX9X
#include<d3d9.h>
#include<d3dx9.h>

#include<Windows.h>
#include<string>
#include"MyTypes.h"

namespace Inferno{
	typedef IDirect3DTexture9* LP_TEXTURE;

class GraphicsDirectX9X : public IGraphics
{
public:
	GraphicsDirectX9X(HWND hWnd);
	~GraphicsDirectX9X() noexcept override;

	void Clear(int r, int g, int b) override;
	bool BeginScene() const;
	void EndScene() const;
	bool BeginSprite() const;
	void EndSprite() const;

	LPDIRECT3DDEVICE9 GetDevice() const;
	LPD3DXSPRITE GetSprite() const;

	/*bool LoadTexture(const std::wstring& filename,
		unsigned* width, unsigned* height, LP_TEXTURE* tex) const;*/

	//void TransformSprite(const Matrix* m) const;

	void DrawSprite(const LP_TEXTURE tex,Vec2<float> center,const int alpha=255) const;

private:
	LPDIRECT3D9       m_d3d;
	LPDIRECT3DDEVICE9 m_d3dDevice;

	LPD3DXSPRITE m_sprite;
};
}
#else
//namespace Inferno
//{
//	class GraphicsDirectX9X : public IGraphics
//	{
//	public:
//		GraphicsDirectX9X(HWND) {}
//		~GraphicsDirectX9X() noexcept override {}
//
//		void Set(int, int, int) override final {}
//	};
//}
#endif //USE_DIRECTX9X