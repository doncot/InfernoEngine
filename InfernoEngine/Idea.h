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
		//���ʂ̃f�X�g���N�^�i����͌p�����Ȃ��j
		~Idea();

		//void SetSize(const XMFLOAT3& size) noexcept;
		const DirectX::XMFLOAT3 GetSize() const noexcept;

		//Vec2<int> GetDrawBase() const;
		//�e�N�X�`���̐ݒ��n�����Ƃ��l���Ẵe�N�X�`���N���X�n������
		//�璷�Ȃ̂Ńt�@�C���������ɂ��������悢�i���Ȃ��������H�j
		/*void SetTexture(const Texture& tex);
		const Texture* GetTexture() const;*/

		/*void SetActiveRange(const Rect& screen, int margin);*/

	private:
		//const Texture* m_tex;

		//Vec2<int> m_drawBase; //�`���i�����ł͊G�̒��S�j
		DirectX::XMFLOAT3 m_size;
		//Rect m_activeRange;

#pragma region unnecessary
	//�R�s�[�R���X�g���N�^��������Z�q�͕s�K�v
		Idea(const Idea&) = delete;
		Idea& operator=(const Idea&) = delete;
#pragma endregion
	};
}