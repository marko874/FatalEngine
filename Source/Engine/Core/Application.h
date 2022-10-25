#pragma once

#include <Engine/Core/Logger.h>
#include <FatalPCH.h>
#include <Renderer/Renderer.h>

namespace Application
{
/**
 * A configuration for an application.
 */
struct ApplicationConfig
{
	/**
	 * Start position on the X axis.
	 */
	int16_t m_StartPosX;

	/**
	 * Start position on the Yaxis.
	 */
	int16_t m_StartPosY;

	/**
	 * Window width.
	 */
	int16_t m_StartWidth;

	/**
	 * Window height.
	 */
	int16_t m_StartHeight;

	/**
	 * Title bar of the application.
	 */
	std::string_view m_AppName;
};

class Game
{
public:
	Game() = default;

	inline bool initialize()
	{
		Logger::log<Logger::Level::Info>("Game initialized.");
		return true;
	}

	inline void on_resize([[maybe_unused]] uint32_t width, [[maybe_unused]] uint32_t height)
	{
		Logger::log<Logger::Level::Info>("Game window resized.");
	}

	Application::ApplicationConfig m_Config;
	void*                          m_State;
};

/**
 * Creates the application. Initializes the subsystems and starts the platform.
 *
 * @return bool Success of the application creation.
 */
FATAL_API bool create_application(Game const& game);

/**
 * Runs the application loop.
 */
FATAL_API bool run_application();
} // namespace Application
