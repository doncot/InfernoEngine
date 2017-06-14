#define _USE_MATH_DEFINES //��ԏ�ɂ��K�v����i�v���W�F�N�g�ݒ肩��ł��悢�j
#include<cmath>

#include"Graphics.h"
#include"Substance.h"
#include"GameElementsCommon.h"

using namespace Inferno;
using namespace DirectX;

Substance::Substance(const Idea& idea) :
	m_idea(idea), m_attribute(0), m_angle(0.0f), m_alpha(0xff)
{
	SetAttribute(GEAttribute::Visible, true);
}

Substance::~Substance() {}

void Substance::AMove(const float x, const float y)
{
	if (CheckAttribute(GEAttribute::UserLock)) return;

	m_pos.x = x;
	m_pos.y = y;
}

void Substance::AMove(const XMFLOAT3& c)
{
	AMove(c.x, c.y);
}

void Substance::RMove(const float x, const float y)
{
	if (CheckAttribute(GEAttribute::UserLock)) return;

	m_pos.x += x;
	m_pos.y += y;
}

void Substance::RMove(const XMFLOAT3& c)
{
	RMove(c.x, c.y);
}

//bool Substance::SmartMove(const Vec2<int>& dst, const double approachScale,
//	Animation::TransitType ttype)
//{
//	Vec2<double> d;
//	bool xhasfinished = false;
//	bool yhasfinished = false;
//
//	switch (ttype)
//	{
//	case Animation::Linear:
//		//x���t�߂������ꍇ�A�W���X�g�Ɂi�Ȃ�Ɖ����t���O���j
//		if ( (dst.x - approachScale) < m_pos.x &&  m_pos.x < (dst.x + approachScale))
//		{
//			m_pos.x = dst.x;
//			xhasfinished = true;
//		}
//		if ( (dst.y - approachScale) < m_pos.y &&  m_pos.y < (dst.y + approachScale))
//		{
//			m_pos.y = dst.y;
//			yhasfinished = true;
//		}
//		if (xhasfinished && yhasfinished) return true;
//
//		//approachScale�����߂Â�
//		m_pos.x > dst.x ? m_pos.x -= approachScale : m_pos.x += approachScale;
//		m_pos.y > dst.y ? m_pos.y -= approachScale : m_pos.y += approachScale;
//
//		break;
//
//	case Animation::EaseOut:
//		d.x = (dst.x - m_pos.x) / approachScale;
//		d.y = (dst.y - m_pos.y) / approachScale;
//		m_pos.x += d.x;
//		m_pos.y += d.y;
//		break;
//
//	default:
//		return false;
//	}
//
//	if (m_pos == dst) return true;
//	else return false;
//}

//������W�ňʒu���w��
//void Substance::SetPosofULCorner(const int x, const int y)
//{
//	if (CheckAttribute(GEAttribute::UserLock)) return;
//	m_pos.x = x + m_idea->GetWidth()/2;
//	m_pos.y = y + m_idea->GetHeight()/2;
//}

void Substance::Rotate(const float angle) {	m_angle = angle; }

XMFLOAT3 Substance::GetPosition() const noexcept
{
	return m_pos;
}

//Rect Substance::GetRegion() const
//{
//	Rect r(m_pos, m_idea->GetHeight(), m_idea->GetWidth());
//	return r;
//}

//Rect Substance::GetHitBox() const
//{
//	Rect r(m_pos, m_idea->GetHeight(), m_idea->GetWidth());
//	return r;
//}


bool Substance::CheckAttribute(GEAttribute attr) const noexcept
{
	return (m_attribute & static_cast<unsigned char>(attr)) ? true : false;
}

void Substance::SetAttribute(GEAttribute attr, bool value) noexcept
{
	//�r�b�g�𗧂Ă�ꍇ
	if (value)
	{
		m_attribute |= static_cast<unsigned char>(attr);
	}
	//�r�b�g���~�낷�ꍇ
	else
	{
		m_attribute &= ~static_cast<unsigned char>(attr);
	}
}

void Substance::SetAlpha(const int alpha) noexcept
{
	m_alpha = alpha;
}

void Substance::Update()
{
	/*if (this->AmIOutOfRange())
	{
		this->SetAttribute(GEAttribute::KillMe, true);
	}*/
}

void Substance::Draw(const IGraphics& g) const
{
	//�`���ԂłȂ��ꍇ�A�A��
	if (!CheckAttribute(GEAttribute::Visible)) return;

	//D3DXMATRIX matWorld; //����������o�ɂ���Ώȗ��ł��邪���Ă���
	//D3DXMatrixIdentity(&matWorld);
	////�ړ�
	////��������s�N�Z�����W(int)���X�N���[�����W(float)�֕ϊ�������
	//matWorld._41 = static_cast<float>(m_pos.x);
	//matWorld._42 = static_cast<float>(m_pos.y);
	////��]
	//float radian = m_angle * M_PI / 180;
	//matWorld._11 = std::cosf(radian);
	//matWorld._12 = std::sinf(radian);
	//matWorld._21 = -std::sinf(radian);
	//matWorld._22 = std::cosf(radian);

	////�l���Z�b�g
	//g.GetSprite()->SetTransform(&matWorld);

	//g.DrawSprite(m_idea->GetTexture()->GetTexture(),
	//	Vec2<float>( static_cast<float>(m_idea->GetDrawBase().x),static_cast<float>(m_idea->GetDrawBase().y) ),
	//	m_alpha
	//	);
}

Substance& Substance::operator=(const Substance& s)
{
	//�������g������\�����l������
	if (&s != this)
	{
		m_pos = s.m_pos;
		//�C�f�A�͓����i�萔�Ȃ̂ɒ��Ӂj
	}
	return *this;
}

//private
//bool Substance::AmIOutOfRange() const
//{
//	//�͈͂��ݒ肳��ĂȂ��Ȃ疳��
//	if (m_idea->m_activeRange.Width() == 0) return false;
//	//��ʊO�Ȃ�
//	if (!IsPointInsideRect(m_pos, m_idea->m_activeRange))
//	{
//		return true;
//	}
//	else return false;
//}
