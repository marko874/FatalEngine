#include "Backend.h"

#include <Core/Asserts.h>
#include <Core/Logger.h>
#include <Core/PlatformLayer.h>
#include <vector>
#include <vulkan/vulkan.h>

namespace Renderer
{
struct RendererBackend
{
    VkInstance m_Instance;
    uint64_t m_FrameNumber;
};

static Renderer::RendererBackend backend;

auto fatal_vk_assert = [](VkResult res) -> void {
    if (res != VK_SUCCESS)
    {
        Logger::log(Logger::Level::Error, {"fatal_vk_assert() failed.\n"});
        debug_break();
    }
};

bool initialize(std::string_view app_name)
{
    using enum Logger::Level;

    backend.m_FrameNumber = 0;

    VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = app_name.data(),
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "FatalEngine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_3,
    };

    std::vector<const char *> extensions = {VK_KHR_SURFACE_EXTENSION_NAME};
    extensions.emplace_back(Platform::get_vulkan_extension_name());

#ifdef _DEBUG
    extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    for (const auto &ext : extensions)
    {
        Logger::log(Debug, {ext, "\n"});
    }
#endif

    std::vector<const char *> layers;

#ifdef _DEBUG
    Logger::log(Info, {"Validation layers enabled.\n"});

    layers.emplace_back("VK_LAYER_KHRONOS_validation");

    uint32_t available_layer_count = 0;
    fatal_vk_assert(vkEnumerateInstanceLayerProperties(&available_layer_count, nullptr));

    std::vector<VkLayerProperties> available_layers(available_layer_count);
    fatal_vk_assert(vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers.data()));

    // TODO: check if validation layer exists

#endif
    VkInstanceCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = static_cast<uint32_t>(layers.size()),
        .ppEnabledLayerNames = layers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
        .ppEnabledExtensionNames = extensions.data(),
    };

    fatal_vk_assert(vkCreateInstance(&create_info, nullptr, &backend.m_Instance));

    Logger::log(Info, {"Vulkan renderer initialized successfully.\n"});
    return true;
} // namespace Renderer

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
