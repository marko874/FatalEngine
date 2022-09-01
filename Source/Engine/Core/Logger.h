#pragma once

#include "../Defines.h"
#ifdef _WIN32
#include "WindowsLayer.h"
#endif
namespace Logger
{
/**
 * Levels of the logging. Each has a distcint color.
 */
enum class Level
{
    Fatal,
    Error,
    Warn,
    Info,
    Debug,
    Trace,
};

/**
 * Initializes the logger subsystem.
 *
 * @return bool Success of the initialization.
 */
bool initialize_logger();

/**
 * Shuts down the logger.
 */
void shutdown_logger();

/**
 * Log to the console.
 *
 * @param lvl The urgency of the message which will be logged. Can either be
 * Fatal, Error, Warn, Info, Debug or Trace.
 * @param list An initializer list of const char* to log.
 */

template <Level lvl, typename... T> inline void log(T &&...args)
{
    static constexpr const char *LEVELS[6] = {
        "[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};

    const uint8_t lvl_as_int = static_cast<const uint8_t>(lvl);

    DWORD handle = (lvl_as_int < 2 ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE);

    Platform::write_to_console<lvl_as_int>(handle, LEVELS[lvl_as_int], args..., "\n");
}
} // namespace Logger
