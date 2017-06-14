/*
 �O���t�B�b�N�X�̃C���N���[�h
*/

#pragma once
#include<memory>
#include<cassert>

namespace Inferno
{
//#define USE_DIRECTX9X

	/// <summary>
	/// ���p����O���t�B�b�NAPI�̎��
	/// </summary>
	enum class GraphicApi
	{
		DirectX9X,
		DirectX10
	};

	/// <summary>
	/// �O���t�B�b�N�X�̃C���^�[�t�F�[�X
	/// </summary>
	class IGraphics
	{
	public:
		virtual ~IGraphics() noexcept = 0 {}

		/// <summary>
		/// �w��F�ŃN���A
		/// </summary>
		virtual void SetBackgroundColor(int r, int g, int b, int a = 255) = 0;
		virtual void SetBackgroundColor(float r, float g, float b, float a = 1.0f) = 0;

		virtual void Draw() = 0;
	};

	///// <summary>
	///// �O���t�B�b�N�X�̃t�@�N�g���[
	///// </summary>
	//class Graphics
	//{
	//public:
	//	/// <summary>
	//	/// �O���t�B�b�N�X�𐶐�����B
	//	/// </summary>
	//	/// <param name="apiType">�O���t�B�N�X�̎��</param>
	//	/// <param name="hWnd">�E�B���h�E�n���h��</param>
	//	/// <returns>�O���t�B�N�X�C���X�^���X�̃C���^�[�t�F�[�X</returns>
	//	static std::shared_ptr<IGraphics> Create(GraphicApi apiType, HWND hWnd);
	//};
}

#include"GraphicsDirectX11.h"