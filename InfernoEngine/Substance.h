#pragma once
#include"GameElementsCommon.h"
#include"GraphicsDirectX11.h"
#include"Idea.h"

using namespace DirectX;

namespace Inferno
{
	//ゲームの個々要素
	//二次元座標で動作すること前提
	class Substance
	{
	public:
		Substance(const Idea& idea); 
		virtual ~Substance() noexcept;

		/// <summary>
		/// 絶対座標指定移動
		/// </summary>
		virtual void AMove(const float x, const float y);
		/// <summary>
		/// 絶対座標指定移動
		/// </summary>
		virtual void AMove(const XMFLOAT3& c);
		//相対標指定移動
		virtual void RMove(const float x, const float y);
		virtual void RMove(const XMFLOAT3& c);

		////dstへ移動。approachScaleで接近する早さをコントロール（小さいほど早い）
		////移動が完了したらtrueを返す
		//bool SmartMove(const Vec2<int>& dst, const double approachScale,
		//	Animation::TransitType ttype);

		//左上座標で位置を指定
		//void SetPosofULCorner(const int x, const int y);

		////時計周りに回転（度数指定）
		void Rotate(const float degree);

		XMFLOAT3 GetPosition() const noexcept;

		//位置座標とサイズから現在領域を求める
		//Rect GetRegion() const;
		/*Rect GetHitBox() const;*/


		//属性（描画可能など）を指定
		void SetAttribute(GEAttribute attr, bool value) noexcept;
		bool CheckAttribute(GEAttribute attr) const noexcept;

		void SetAlpha(const int alpha) noexcept;

		//DispatchMessage(GMessageQueue)
		//InterpretMessage(GMessageQueue);

		virtual void Update();

		void Draw(const GraphicsDirectX11& g) const;

		//コピーコンストラクタ＆代入演算子必要
		Substance& operator=(const Substance& s);

	private:
		//画面外にいるか探知
		//bool AmIOutOfRange() const;

	protected:
		const Idea& m_idea;
		XMFLOAT3 m_pos; //現在座標
		float m_angle; //度数法で回転情報
		int m_alpha;
		unsigned char m_attribute; //属性（描画属性など）を指定
	};
}