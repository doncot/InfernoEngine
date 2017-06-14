#pragma once

#include"Graphics.h"
//#include<Texture.h>

namespace Inferno
{
	class Idea
	{
	public:
		//friend class Substance;
		Idea();
		//普通のデストラクタ（これは継承しない）
		~Idea();

		//void SetSize(const XMFLOAT3& size) noexcept;
		const DirectX::XMFLOAT3 GetSize() const noexcept;

		//Vec2<int> GetDrawBase() const;
		//テクスチャの設定を渡すことを考えてのテクスチャクラス渡しだが
		//冗長なのでファイル名だけにした方がよい（←なぜだっけ？）
		/*void SetTexture(const Texture& tex);
		const Texture* GetTexture() const;*/

		/*void SetActiveRange(const Rect& screen, int margin);*/

	private:
		//const Texture* m_tex;

		//Vec2<int> m_drawBase; //描画基準（ここでは絵の中心）
		DirectX::XMFLOAT3 m_size;
		//Rect m_activeRange;

#pragma region unnecessary
	//コピーコンストラクタ＆代入演算子は不必要
		Idea(const Idea&) = delete;
		Idea& operator=(const Idea&) = delete;
#pragma endregion
	};
}