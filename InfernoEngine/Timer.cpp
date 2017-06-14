#include"Timer.h"

#include<Windows.h>
#include<mmsystem.h> // timeGetTime用
#pragma comment(lib, "winmm.lib") // VC++only

namespace Inferno
{
	void Timer::Start()
	{
		m_start = timeGetTime();
	}

	void Timer::Stop()
	{
		m_start = 0;
	}

	Millisec Timer::GetElapsed() const
	{
		if (!HasStarted()) return 0; //始まってない場合、0を返す
		return timeGetTime() - m_start;
	}

	bool Timer::HasStarted() const
	{
		return m_start != 0 ? true : false;
	}

	void Timer::Pause()
	{
		//まだポーズタイマーを使ったことがない場合
		if (!m_pauseTimer)
		{
			m_pauseTimer = new Timer();
		}
		m_pauseTimer->Start();
	}

	void Timer::UnPause()
	{
		//経過時間だけ巻き戻す
		m_start -= m_pauseTimer->GetElapsed();
		//ここで止めないと二回目に呼んだときに二重に引いてしまう
		m_pauseTimer->Stop();
	}

	Stopwatch::Stopwatch() { m_targetTime = 0; }

	Stopwatch::Stopwatch(const Millisec msec)
	{
		this->Start(msec);
	}

	bool Stopwatch::HasFinished() const
	{
		return m_timer.GetElapsed() > m_targetTime;
	}

	bool Stopwatch::HasStarted() const
	{
		return m_timer.HasStarted();
	}

	void Stopwatch::Start(const Millisec msec)
	{
		m_timer.Start();
		m_targetTime = msec;
	}

	void Stopwatch::Restart()
	{
		m_timer.Start();
	}

	double Stopwatch::DurationTimeRate() const
	{
		return static_cast<double>(m_timer.GetElapsed()) / m_targetTime;
	}

}