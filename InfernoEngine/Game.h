/*
Game.h
�Q�[���{��
����͊��Ŋe�Q�[�����Ƃɔh��������
*/
#pragma once
#include"SimpleWindow.h"
#include"Input.h"
#include"Timer.h"
#include<memory>
#include<deque>

namespace Inferno
{
	//�O���錾
	class GraphicsDirectX11;

	class Game : public SimpleWindow
	{
	public:
		Game();
		virtual ~Game() override;

		virtual bool GameLoop();
		virtual void Exit();

		/// <summary>
		/// �N���C�A���g�̈��傫����ݒ�
		/// </summary>
		/// <param name="logicalClientWidth">DPI�X�P�[�����O���l�����Ȃ��_���I�ȕ�</param>
		/// <param name="logicalClientHeight">DPI�X�P�[�����O���l�����Ȃ��_���I�ȍ���</param>
		void Resize(const int logicalClientWidth, const int logicalClientHeight) override final;

		/// <summary>
		/// GetFps�Ŏg���l���v�Z����B
		/// </summary>
		/// <remarks>GetFps���g���ꍇ�A���t���[���Ăяo���B</remarks>
		void SampleFps();
		const float GetFps() const;

		/// <summary>
		/// �t���X�N���[�����[�h�̐؂�ւ�
		/// </summary>
		/// <param name="modeFlag">true�Ńt���X�N���[���Afalse�ŃE�B���h�E�B</param>
		void SetFullScreenState(bool isFullscreen);

	protected:
		std::unique_ptr<GraphicsDirectX11> m_pGraphics = nullptr;
		Input m_input;

		unsigned int m_TargetFps = 60;

		long long int m_elapsedFrame; //�Q�[���J�n����̌o�߃t���[��
		long long int m_frameCount; //�O��̃��[�v����̌o�߃t���[��

	private:
#pragma region FPS
		/// <summary>
		/// �Q�[���S�̂̃^�C�}�[
		/// </summary>
		mutable Timer m_gameTimer;

		/// <summary>
		/// �O��t���[������̌o�ߎ��Ԃ��v��^�C�}�[
		/// </summary>
		Timer m_previousFrameTimer;

		/// <summary>
		/// FPS�̕��ς��v������T���v����
		/// </summary>
		static constexpr size_t PreviousFrameTimeSampleCount = 16;
		/// <summary>
		/// FPS�̕��ς��v������T���v��
		/// </summary>
		std::deque<Millisec> m_previousFrameTimeStorage;
#pragma endregion FPS
	};
}