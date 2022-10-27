#pragma once

#include <vulkan/vulkan.h>

namespace Editor
{
void init_ui(VkDevice const& device, VkPhysicalDevice const& physical, void* hwnd, VkQueue const& queue, VkRenderPass const& render_pass, VkInstance const& instance) noexcept;
} // namespace Editor
