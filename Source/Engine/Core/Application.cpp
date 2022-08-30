#include "Application.h"
#include "Logger.h"
#include "PlatformLayer.h"

struct ApplicationState
{
    bool m_IsRunning;
    bool m_IsSuspended;
    int16_t m_Width;
    int16_t m_Height;
    float m_DeltaTime;

    Platform::PlatformState m_PlatformState;
    Application::Game m_Game;
};

static ApplicationState app_state;
static bool is_initialized = false;

namespace Application
{

bool create_application(Game const &game)
{
    if (is_initialized)
    {
        Logger::log(Logger::Level::Error, {"App created more than once.\n"});
        return false;
    }

    app_state.m_Game = game;

    // Subsystems initialization
    Logger::initialize_logger();

    app_state.m_IsRunning = true;
    app_state.m_IsSuspended = false;

    if (!app_state.m_PlatformState.start_platform(game.m_Config.m_AppName, game.m_Config.m_StartPosX,
                                                  game.m_Config.m_StartPosY, game.m_Config.m_StartWidth,
                                                  game.m_Config.m_StartHeight))
    {
        return false;
    }

    if (!app_state.m_Game.initialize())
    {
        Logger::log(Logger::Level::Fatal, {"Game initialization failed.\n"});
        return false;
    }

    is_initialized = true;
    return true;
}

bool run_application()
{
    while (app_state.m_IsRunning)
    {
        if (!app_state.m_PlatformState.pump_messages())
        {
            app_state.m_IsRunning = false;
        }

        if (!app_state.m_IsSuspended)
        {
            if (!app_state.m_Game.update(0.0f))
            {
                Logger::log(Logger::Level::Fatal, {"Game update failed.\n"});
                app_state.m_IsRunning = false;
                break;
            }

            if (!app_state.m_Game.render(0.0f))
            {
                Logger::log(Logger::Level::Fatal, {"Game render failed.\n"});
                app_state.m_IsRunning = false;
                break;
            }
        }
    }

    app_state.m_IsRunning = false;
    return true;
}
} // namespace Application
