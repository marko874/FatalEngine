#include "Clock.h"

#include <chrono>

namespace Fatal
{
using namespace std::chrono;

void Clock::start() noexcept
{
	m_StartTime   = now();
	m_ElapsedTime = 0;
}

void Clock::update() noexcept
{
	if(m_StartTime != 0)
	{
		m_ElapsedTime = now() - m_StartTime;
	}
}

void Clock::stop() noexcept
{
	m_StartTime = 0;
}

long long Clock::get_elapsed_time() const noexcept
{
	return m_ElapsedTime;
}

long long Clock::now() const noexcept
{
	return duration_cast<milliseconds>(time_point_cast<milliseconds>(high_resolution_clock::now()).time_since_epoch()).count();
}
} // namespace Fatal
