#pragma once

namespace Fatal
{
class Clock
{
public:
	Clock() = default;

	void start() noexcept;

	void update() noexcept;

	void stop() noexcept;

	[[nodiscard]] long long get_elapsed_time() const noexcept;

	[[nodiscard]] long long now() const noexcept;

private:
	long long m_StartTime;
	long long m_ElapsedTime;
};
} // namespace Fatal
