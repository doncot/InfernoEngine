/*
Game.h
ゲーム本体
これは基底で各ゲームごとに派生させる
*/
#pragma once
#include"SimpleWindow.h"
#include"Input.h"
#include"Timer.h"
#include<memory>
#include<deque>

namespace Inferno
{
	//前方宣言
	class GraphicsDirectX11;

	class Game : public SimpleWindow
	{
	public:
		Game();
		virtual ~Game() override;

		virtual bool GameLoop();
		virtual void Exit();

		/// <summary>
		/// クライアント領域を大きさを設定
		/// </summary>
		/// <param name="logicalClientWidth">DPIスケーリングを考慮しない論理的な幅</param>
		/// <param name="logicalClientHeight">DPIスケーリングを考慮しない論理的な高さ</param>
		void Resize(const int logicalClientWidth, const int logicalClientHeight) override final;

		/// <summary>
		/// GetFpsで使う値を計算する。
		/// </summary>
		/// <remarks>GetFpsを使う場合、毎フレーム呼び出す。</remarks>
		void SampleFps();
		const float GetFps() const;

		/// <summary>
		/// フルスクリーンモードの切り替え
		/// </summary>
		/// <param name="modeFlag">trueでフルスクリーン、falseでウィンドウ。</param>
		void SetFullScreenState(bool isFullscreen);

	protected:
		std::unique_ptr<GraphicsDirectX11> m_pGraphics = nullptr;
		Input m_input;

		unsigned int m_TargetFps = 60;

		long long int m_elapsedFrame; //ゲーム開始からの経過フレーム
		long long int m_frameCount; //前回のループからの経過フレーム

	private:
#pragma region FPS
		/// <summary>
		/// ゲーム全体のタイマー
		/// </summary>
		mutable Timer m_gameTimer;

		/// <summary>
		/// 前回フレームからの経過時間を計るタイマー
		/// </summary>
		Timer m_previousFrameTimer;

		/// <summary>
		/// FPSの平均を計測するサンプル数
		/// </summary>
		static constexpr size_t PreviousFrameTimeSampleCount = 16;
		/// <summary>
		/// FPSの平均を計測するサンプル
		/// </summary>
		std::deque<Millisec> m_previousFrameTimeStorage;
#pragma endregion FPS
	};
}