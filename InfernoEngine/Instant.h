#pragma once
#include"GameElementsCommon.h"
#include"Idea.h"
#include"Substance.h"

namespace Inferno
{
	//単数しかない要素の場合、いちいちイデアから実体を作るのが面倒なのでこっちを使う
	//中身はイデアを内部に持つ実体である。
	class Instant : public Substance
	{
	public:
		Instant();
		virtual ~Instant() noexcept;

		/*void LoadTextureFromFile(const Graphics& g, const wstring& str)
		{
		m_tex->LoadImageFile(g, str);
		m_pIdea->SetTexture(*m_tex);
		Base::m_pIdea = m_pIdea;
		}*/

	private:
		//自分で書き込む必要があるので、constなsubstaceのideaは使えない
		const std::unique_ptr<Idea> m_pIdea;
		//Texture* m_tex;
	};
}