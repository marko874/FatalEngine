#include <EntryPoint.h>

bool create_game(Application::Game &out_game)
{
    out_game.m_Config.m_StartPosX = 100;
    out_game.m_Config.m_StartPosY = 100;
    out_game.m_Config.m_StartWidth = 1920;
    out_game.m_Config.m_StartHeight = 1080;
    out_game.m_Config.m_AppName = "FatalEngine";

    out_game.m_State = new float;
    delete out_game.m_State;

    return true;
}
