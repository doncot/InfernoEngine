#pragma once

#include"WindowsCommon.h"

using Millisec = unsigned long long int;
const int FixFPS = 60;

namespace Inferno
{
	class Timer
	{
	public:
		Timer(){};
		~Timer()
		{
			SAFE_DELETE(m_pauseTimer);
		}

		void Start();
		void Stop();
		Millisec GetElapsed() const;
		void Pause();
		void UnPause();

		bool HasStarted() const;

	private:
		Millisec m_start = 0;
		Timer* m_pauseTimer = nullptr;
	};

	//�w�莞�Ԍo�߂��Ă��邩�ǂ����𔻒肷��N���X
	class Stopwatch
	{
	public:
		Stopwatch();
		Stopwatch(const Millisec msec);
		bool HasFinished() const;
		bool HasStarted() const;
		void Start(const Millisec msec);
		//�����ݒ莞�ԂōċN��
		void Restart();
		double DurationTimeRate() const;

	private:
		Timer m_timer;
		Millisec m_targetTime;
	};
}