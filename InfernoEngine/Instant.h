#pragma once
#include"GameElementsCommon.h"
#include"Idea.h"
#include"Substance.h"

namespace Inferno
{
	//�P�������Ȃ��v�f�̏ꍇ�A���������C�f�A������̂����̂��ʓ|�Ȃ̂ł��������g��
	//���g�̓C�f�A������Ɏ����̂ł���B
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
		//�����ŏ������ޕK�v������̂ŁAconst��substace��idea�͎g���Ȃ�
		const std::unique_ptr<Idea> m_pIdea;
		//Texture* m_tex;
	};
}