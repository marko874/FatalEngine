#pragma once

extern "C"
{
#pragma warning(push, 0)
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#pragma warning(pop)
}

#include <string_view>

namespace Scripting
{

class State
{
public:
	State();
	virtual ~State();

	int32_t process_file(std::string_view file) const noexcept;
	bool    check_expression(int32_t exp) const noexcept;

	State(State&&)                 = delete;
	State(State const&)            = delete;
	State& operator=(State&&)      = delete;
	State& operator=(State const&) = delete;

private:
	lua_State* m_state;
};
} // namespace Scripting
