#include"Idea.h"
#include"GameElementsCommon.h"
#include"Graphics.h"

using namespace Inferno;
using namespace DirectX;

Idea::Idea() {}
Idea::~Idea() {}

//void Idea::SetTexture(const Texture& tex)
//{
//	m_tex = &tex;
//	//�`���̌v�Z
//	m_width = m_tex->GetWidth();
//	m_height = m_tex->GetHeight();
//
//	m_drawBase.x = m_width / 2;
//	m_drawBase.y = m_height / 2;
//}

//const Texture* Idea::GetTexture() const
//{
//	return m_tex;
//}

//Vec2<int> Idea::GetDrawBase() const
//{
//	return m_drawBase;
//}

//void Idea::SetSize(const XMFLOAT3& size) noexcept
//{
//	m_size = size;
//}

const XMFLOAT3 Idea::GetSize() const noexcept
{
	return m_size;
}

//void Idea::SetActiveRange(const Rect& screen, int margin)
//{
//	//�����s���Ė߂�P�[�X������̂ŁA�������߂ɉ�ʊO�ɂ͂ݏo�����猟�o
//	Rect tempRect;
//	tempRect.SetSize(screen.Width() + margin * 2, screen.Height() + margin * 2);
//	//�E��ɃZ�b�g
//	tempRect.SetPosofULCorner(0, 0);
//	m_activeRange = tempRect;
//}
