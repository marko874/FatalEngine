#pragma once

#include <Engine/Core/Application.h>
#include <Engine/Core/Logger.h>

/**
 * Creates the game by specifying the starting positions, size of the window and the titlebar.
 * Handled by the user-code (in the executables).
 *
 * @param out_game A Game instance which the user specified configuration will be written to.
 *
 * @return bool Success of the game creation.
 */
extern bool create_game(Application::Game &out_game);

int main()
{
    Application::Game game;
    if (!create_game(game))
    {
        Logger::log<Logger::Level::Fatal>("Game creation failed.");
    }

    if (!Application::create_application(game))
    {
        Logger::log<Logger::Level::Info>("Application creation failed.");
        return 1;
    }

    if (!Application::run_application())
    {
        Logger::log<Logger::Level::Info>("Application didn't shut down properly.");
    }
}
