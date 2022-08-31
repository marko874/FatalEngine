#pragma once

#include <Engine/Core/Logger.h>
#include <FatalPCH.h>

namespace Application
{
/**
 * A configuration for an application.
 */
#pragma warning(disable : 26495)
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
        Logger::log(Logger::Level::Info, {"Game initialized.\n"});
        return true;
    }

#pragma warning(disable : 4100)
    inline bool update(double dt)
    {
        return true;
    }

#pragma warning(disable : 4100)
    inline bool render(double dt)
    {
        return true;
    }

#pragma warning(disable : 4100)
    inline void on_resize(uint32_t width, uint32_t height)
    {
        Logger::log(Logger::Level::Info, {"Game window resized.\n"});
    }

    Application::ApplicationConfig m_Config;
    void *m_State;
};

/**
 * Creates the application. Initializes the subsystems and starts the platform.
 *
 * @return bool Success of the application creation.
 */
FATAL_API bool create_application(Game const &game);

/**
 * Runs the application loop.
 */
FATAL_API bool run_application();
} // namespace Application
