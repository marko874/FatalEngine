#include "Renderer.h"

#include <Core/Asserts.h>
#include <Core/Logger.h>

namespace Utils
{
using namespace Assert;
[[nodiscard]] static std::vector<VkLayerProperties> get_layer_properties() noexcept
{
    uint32_t available_layer_count = 0;
    fatal_vk_assert(vkEnumerateInstanceLayerProperties(&available_layer_count, nullptr));

    std::vector<VkLayerProperties> available_layers(available_layer_count);
    fatal_vk_assert(vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers.data()));

    return available_layers;
}

bool validation_layers_supported(std::span<const char *> requested_layers) noexcept
{
    bool found = false;
    auto available_layers = get_layer_properties();

    for (const auto &requested_layer : requested_layers)
    {
        for (const auto &available_layer : available_layers)
        {

            if (strcmp(available_layer.layerName, requested_layer) == 0)
            {
                found = true;
                Logger::log<Logger::Level::Info>("Found requested layer!");
                return found;
            }
        }
    }
    return found;
}

[[nodiscard]] VkInstance create_instance(const char *app_name) noexcept
{
    VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = app_name,
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "FatalEngine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_3,
    };

    std::vector<const char *> layers;
#ifdef _DEBUG
    layers.emplace_back("VK_LAYER_KHRONOS_validation");

    if (!validation_layers_supported(layers))
    {
        layers.clear();
    }
#endif

    const std::vector<const char *> extensions = {VK_KHR_SURFACE_EXTENSION_NAME, Platform::get_vulkan_extension_name()};

    VkInstanceCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = static_cast<uint32_t>(layers.size()),
        .ppEnabledLayerNames = layers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
        .ppEnabledExtensionNames = extensions.data(),
    };

    VkInstance instance;
    fatal_vk_assert(vkCreateInstance(&create_info, nullptr, &instance));

    return instance;
}
} // namespace Utils
