#pragma once

#include <FatalPCH.h>
#include <vulkan/vulkan.h>

namespace Platform
{
static constexpr uint8_t LEVELS[6] = { 64, 4, 6, 2, 1, 8 };

/**
 * An OS-agnostic platform layer class. Currently implemented only for Windows.
 */
class PlatformState
{
public:
	/**
	 * Default constructor.
	 */
	PlatformState() = default;

	/**
	 * Start the platform.
	 *
	 * @param app_name Title bar of the window.
	 * @param x Start position of the window on the X axis.
	 * @param y Start position of the window on the Y axis.
	 * @param width Width of the window.
	 * @param height Height of the window.
	 *
	 * @return bool Success of the start-up.
	 */
	bool start_platform(std::string_view app_name, int32_t x, int32_t y, int32_t width, int32_t height);

	/**
	 * Shut down the platform. This implies the window destruction.
	 */
	bool shut_down_platform();

	/**
	 * Pumps the messages. For Windows specifically,
	 * WinAPI checks the thread message queue, retrieves the messages,
	 * translates them into character messages and dispatches them to window procedure.
	 */
	bool pump_messages();

	void* get_state() const noexcept;

	/**
	 * Virtual destructor. Shuts down the platform
	 * and deletes the underlying dynamically allocated member variable.
	 */
	virtual ~PlatformState();

private:
	void* m_InternalState;
};

/**
 * Get the absolute time.
 *
 * @return float Time.
 */
float get_time();

/**
 * Sleep for N miliseconds.
 *
 * @param miliseconds Time in miliseconds to sleep.
 */
void sleep(unsigned long miliseconds);

void* platform_allocate(uint64_t size, bool aligned);

void platform_free(void* block, bool aligned);

void* platform_zero_memory(void* block, uint64_t size);

void* platform_copy_memory(void* destination, void const* source, uint64_t size);

void* platform_set_memory(void* destination, int32_t value, uint64_t size);

const char* get_vulkan_extension_name();

VkSurfaceKHR create_vulkan_surface(VkInstance const& instance, void* state);
} // namespace Platform
