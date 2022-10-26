#include "ScriptingSystem.h"

namespace Scripting
{

State::State()
	: m_state(luaL_newstate())
{
}

State::~State()
{
	lua_close(m_state);
}

void State::process_file(std::string_view file) const noexcept
{
	luaL_dofile(m_state, file.data());
}
} // namespace Scripting
