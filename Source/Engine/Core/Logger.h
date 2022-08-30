#pragma once

#include "../Defines.h"
#include <initializer_list>

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
FATAL_API void log(Level lvl, std::initializer_list<const char *> list);
} // namespace Logger
