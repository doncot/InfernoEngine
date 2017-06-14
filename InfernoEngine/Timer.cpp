#include"Timer.h"

#include<Windows.h>
#include<mmsystem.h> // timeGetTime�p
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
		if (!HasStarted()) return 0; //�n�܂��ĂȂ��ꍇ�A0��Ԃ�
		return timeGetTime() - m_start;
	}

	bool Timer::HasStarted() const
	{
		return m_start != 0 ? true : false;
	}

	void Timer::Pause()
	{
		//�܂��|�[�Y�^�C�}�[���g�������Ƃ��Ȃ��ꍇ
		if (!m_pauseTimer)
		{
			m_pauseTimer = new Timer();
		}
		m_pauseTimer->Start();
	}

	void Timer::UnPause()
	{
		//�o�ߎ��Ԃ��������߂�
		m_start -= m_pauseTimer->GetElapsed();
		//�����Ŏ~�߂Ȃ��Ɠ��ڂɌĂ񂾂Ƃ��ɓ�d�Ɉ����Ă��܂�
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