#pragma once

namespace Fatal
{
class Clock
{
public:
	Clock() = default;

	void start();

	void update();

	void stop();

	[[nodiscard]] double get_elapsed_time() const noexcept;

private:
	double m_StartTime;
	double m_ElapsedTime;
};
} // namespace Fatal
