#pragma once

#include <string_view>

namespace Renderer
{
bool initialize(std::string_view app_name);

void shutdown();

void resized();

bool begin_frame();

bool end_frame();
} // namespace Renderer
