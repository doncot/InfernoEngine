#pragma once
#include"GameElementsCommon.h"
#include"GraphicsDirectX11.h"
#include"Idea.h"

using namespace DirectX;

namespace Inferno
{
	//�Q�[���̌X�v�f
	//�񎟌����W�œ��삷�邱�ƑO��
	class Substance
	{
	public:
		Substance(const Idea& idea); 
		virtual ~Substance() noexcept;

		/// <summary>
		/// ��΍��W�w��ړ�
		/// </summary>
		virtual void AMove(const float x, const float y);
		/// <summary>
		/// ��΍��W�w��ړ�
		/// </summary>
		virtual void AMove(const XMFLOAT3& c);
		//���ΕW�w��ړ�
		virtual void RMove(const float x, const float y);
		virtual void RMove(const XMFLOAT3& c);

		////dst�ֈړ��BapproachScale�Őڋ߂��鑁�����R���g���[���i�������قǑ����j
		////�ړ�������������true��Ԃ�
		//bool SmartMove(const Vec2<int>& dst, const double approachScale,
		//	Animation::TransitType ttype);

		//������W�ňʒu���w��
		//void SetPosofULCorner(const int x, const int y);

		////���v����ɉ�]�i�x���w��j
		void Rotate(const float degree);

		XMFLOAT3 GetPosition() const noexcept;

		//�ʒu���W�ƃT�C�Y���猻�ݗ̈�����߂�
		//Rect GetRegion() const;
		/*Rect GetHitBox() const;*/


		//�����i�`��\�Ȃǁj���w��
		void SetAttribute(GEAttribute attr, bool value) noexcept;
		bool CheckAttribute(GEAttribute attr) const noexcept;

		void SetAlpha(const int alpha) noexcept;

		//DispatchMessage(GMessageQueue)
		//InterpretMessage(GMessageQueue);

		virtual void Update();

		void Draw(const GraphicsDirectX11& g) const;

		//�R�s�[�R���X�g���N�^��������Z�q�K�v
		Substance& operator=(const Substance& s);

	private:
		//��ʊO�ɂ��邩�T�m
		//bool AmIOutOfRange() const;

	protected:
		const Idea& m_idea;
		XMFLOAT3 m_pos; //���ݍ��W
		float m_angle; //�x���@�ŉ�]���
		int m_alpha;
		unsigned char m_attribute; //�����i�`�摮���Ȃǁj���w��
	};
}