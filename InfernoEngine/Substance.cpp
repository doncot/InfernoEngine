#define _USE_MATH_DEFINES //一番上にやる必要あり（プロジェクト設定からでもよい）
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
//		//x軸付近だった場合、ジャストに（なんと汚いフラグか）
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
//		//approachScaleだけ近づく
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

//左上座標で位置を指定
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
	//ビットを立てる場合
	if (value)
	{
		m_attribute |= static_cast<unsigned char>(attr);
	}
	//ビットを降ろす場合
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
	//描画状態でない場合、帰る
	if (!CheckAttribute(GEAttribute::Visible)) return;

	//D3DXMATRIX matWorld; //これをメンバにすれば省略できるがさてさて
	//D3DXMatrixIdentity(&matWorld);
	////移動
	////ここからピクセル座標(int)よりスクリーン座標(float)へ変換をする
	//matWorld._41 = static_cast<float>(m_pos.x);
	//matWorld._42 = static_cast<float>(m_pos.y);
	////回転
	//float radian = m_angle * M_PI / 180;
	//matWorld._11 = std::cosf(radian);
	//matWorld._12 = std::sinf(radian);
	//matWorld._21 = -std::sinf(radian);
	//matWorld._22 = std::cosf(radian);

	////値をセット
	//g.GetSprite()->SetTransform(&matWorld);

	//g.DrawSprite(m_idea->GetTexture()->GetTexture(),
	//	Vec2<float>( static_cast<float>(m_idea->GetDrawBase().x),static_cast<float>(m_idea->GetDrawBase().y) ),
	//	m_alpha
	//	);
}

Substance& Substance::operator=(const Substance& s)
{
	//自分自身がくる可能性を考慮する
	if (&s != this)
	{
		m_pos = s.m_pos;
		//イデアは同じ（定数なのに注意）
	}
	return *this;
}

//private
//bool Substance::AmIOutOfRange() const
//{
//	//範囲が設定されてないなら無視
//	if (m_idea->m_activeRange.Width() == 0) return false;
//	//画面外なら
//	if (!IsPointInsideRect(m_pos, m_idea->m_activeRange))
//	{
//		return true;
//	}
//	else return false;
//}
