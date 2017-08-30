#include "timer.hpp"
#include <iostream>

namespace flounder
{
	timer::timer(const float &interval) :
		m_startTime(0.0f),
		m_interval(interval * 1000.0f)
	{
		if (framework::get() != nullptr)
		{
			m_startTime = framework::get()->getTimeMs();
		}
	}

	timer::~timer()
	{
	}

	bool timer::isPassedTime() const
	{
		return framework::get()->getTimeMs() - m_startTime >= m_interval;
	}

	void timer::resetStartTime()
	{
		m_startTime = framework::get()->getTimeMs();
	}

	float timer::getInterval() const
	{
		return m_interval / 1000.0f;
	}

	void timer::setInterval(const float &interval)
	{
		if (m_interval == interval * 1000.0f)
		{
			return;
		}

		m_interval = interval * 1000.0f;
		m_startTime = framework::get()->getTimeMs();
	}
}
