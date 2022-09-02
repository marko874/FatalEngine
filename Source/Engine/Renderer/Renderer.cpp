#include "Renderer.h"

#include <Core/Asserts.h>
#include <Core/Logger.h>
#include <Core/PlatformLayer.h>
#include <Core/WindowsLayer.h>
#include <vulkan/vulkan.h>

#include <array>
#include <span>

using enum Logger::Level;
using namespace Assert;

namespace Renderer
{
VulkanContext create_context(std::string_view app_name, void *state, uint32_t width, uint32_t height)
{
    VulkanContext ctx = {
        .m_Instance = Utils::create_instance(app_name.data()),
        .m_Surface = Platform::create_vulkan_surface(ctx.m_Instance, state),
        .m_VulkanDevice = Utils::Device::create_device(ctx.m_Instance),
    };

    ctx.m_VulkanSwapchain = Utils::Swapchain::create_swapchain(
        ctx.m_VulkanDevice.m_PhysicalDevice, ctx.m_VulkanDevice.m_Device, ctx.m_Surface, width, height);

    Logger::log<Info>("Renderer initialized successfully.");
    return ctx;
}

void shutdown()
{
}

void resized()
{
}

bool begin_frame()
{
    return true;
}

bool end_frame()
{
    return false;
}
} // namespace Renderer
