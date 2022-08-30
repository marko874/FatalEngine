#include "Logger.h"
#include "../FatalPCH.h"
#include "PlatformLayer.h"

namespace Logger
{
bool initialize_logger()
{
    return true;
}

void shutdown_logger()
{
}

void log(Level lvl, std::initializer_list<const char *> list)
{
    static constexpr std::string_view LEVELS[6] = {
        "[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};

    const uint8_t lvl_as_int = static_cast<const uint8_t>(lvl);
    std::cout << LEVELS[lvl_as_int];

    if (lvl_as_int < 2)
    {
        Platform::write_error_to_console(list, lvl_as_int);
    }
    else
    {
        Platform::write_to_console(list, lvl_as_int);
    }
}
} // namespace Logger
