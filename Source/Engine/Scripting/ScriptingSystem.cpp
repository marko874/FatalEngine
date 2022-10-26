#include "ScriptingSystem.h"

#include <Core/Logger.h>

namespace Scripting
{

State::State()
	: m_state(luaL_newstate())
{
	luaL_openlibs(m_state);
}

State::~State()
{
	lua_close(m_state);
}

int32_t State::process_file(std::string_view file) const noexcept
{
	return luaL_dofile(m_state, file.data());
}

bool State::check_expression(int32_t exp) const noexcept
{
	if(exp != LUA_OK)
	{
		const char* error = lua_tostring(m_state, exp);
		Logger::log<Logger::Level::Error>("Expression failed: ", error);
		return false;
	}

	return true;
}
} // namespace Scripting
