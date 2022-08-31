#include "Clock.h"
#include "PlatformLayer.h"

namespace Fatal
{
void Clock::start()
{
    m_StartTime = Platform::get_time();
    m_ElapsedTime = 0.0;
}

void Clock::update()
{
    if (m_StartTime != 0)
    {
        m_ElapsedTime = Platform::get_time() - m_StartTime;
    }
}

void Clock::stop()
{
    m_StartTime = 0.0;
}

double Clock::get_elapsed_time() const noexcept
{
    return m_ElapsedTime;
}
} // namespace Fatal
