#include "Application.h"
#include "Clock.h"
#include "Logger.h"
#include "PlatformLayer.h"

#include <Renderer/Renderer.h>

#pragma warning(disable : 26495)
struct ApplicationState
{
    bool m_IsRunning;
    bool m_IsSuspended;
    int16_t m_Width;
    int16_t m_Height;

    Fatal::Clock m_Clock;
    double m_LastTime;

    Platform::PlatformState m_PlatformState;
    Application::Game m_Game;
};

static ApplicationState app_state;
static bool is_initialized = false;

namespace Application
{
using enum Logger::Level;

bool create_application(Game const &game)
{
    if (is_initialized)
    {
        Logger::log<Error>("App created more than once.");
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
        Logger::log<Fatal>("Game initialization failed.");
        return false;
    }

    is_initialized = true;
    return true;
}

bool run_application()
{
    app_state.m_Clock.start();
    app_state.m_Clock.update();
    app_state.m_LastTime = app_state.m_Clock.get_elapsed_time();

    double running_time = 0.0;
    uint8_t frame_count = 0;
    double target_frame_seconds = 1.0 / 60.0;

    while (app_state.m_IsRunning)
    {

        if (!app_state.m_PlatformState.pump_messages())
        {
            app_state.m_IsRunning = false;
        }

        if (!app_state.m_IsSuspended)
        {
            app_state.m_Clock.update();
            double current_time = app_state.m_Clock.get_elapsed_time();
            double dt = current_time - app_state.m_LastTime;
            double frame_start_time = Platform::get_time();

            if (!app_state.m_Game.update(dt))
            {
                Logger::log<Fatal>("Game update failed.");
                app_state.m_IsRunning = false;
                break;
            }

            if (!app_state.m_Game.render(dt))
            {
                Logger::log<Fatal>("Game render failed.");
                app_state.m_IsRunning = false;
                break;
            }

            double frame_end_time = Platform::get_time();
            double frame_elapsed_time = frame_end_time - frame_start_time;
            running_time += frame_elapsed_time;

            double remaining_seconds = target_frame_seconds - frame_elapsed_time;

            if (remaining_seconds > 0)
            {
                double remaining_ms = remaining_seconds * 1000.0;

                bool limit_frames = false;
                if (remaining_ms > 0 && limit_frames)
                {
                    Platform::sleep(static_cast<unsigned long>(remaining_ms - 1));
                }

                frame_count++;
            }

            app_state.m_LastTime = current_time;
        }
    }

    app_state.m_IsRunning = false;
    return true;
}
} // namespace Application
