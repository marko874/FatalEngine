#include "Renderer.h"

#include <array>

using namespace Assert;
namespace Utils
{
namespace Device
{
VulkanDevice create_device(VkInstance const &instance) noexcept
{
    VulkanDevice device = {
        .m_PhysicalDevice = get_all_physical_devices(instance)[0],
    };

    constexpr int NUM_PRIORITIES = 1;
    constexpr std::array<float, NUM_PRIORITIES> PRIORITIES = {1.0f};

    auto props = get_queue_family_properties(device.m_PhysicalDevice);
    auto index = select_queue_index(props);

    VkDeviceQueueCreateInfo queue_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = index,
        .queueCount = 1,
        .pQueuePriorities = PRIORITIES.data(),
    };

    VkPhysicalDeviceFeatures features = {};

    constexpr uint32_t NUM_EXTENSIONS = 1;
    constexpr std::array<const char *, NUM_EXTENSIONS> EXTENSIONS = {"VK_KHR_swapchain"};

    VkDeviceCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queue_info,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = NUM_EXTENSIONS,
        .ppEnabledExtensionNames = EXTENSIONS.data(),
        .pEnabledFeatures = &features,
    };

    fatal_vk_assert(vkCreateDevice(device.m_PhysicalDevice, &create_info, nullptr, &device.m_Device));
    vkGetDeviceQueue(device.m_Device, 0, 0, &device.m_Queue);

    return device;
}

std::vector<VkPhysicalDevice> get_all_physical_devices(VkInstance const &instance) noexcept
{
    uint32_t count;
    vkEnumeratePhysicalDevices(instance, &count, nullptr);

    std::vector<VkPhysicalDevice> physical_devices(count);
    vkEnumeratePhysicalDevices(instance, &count, physical_devices.data());

    return physical_devices;
}

std::vector<VkQueueFamilyProperties> get_queue_family_properties(VkPhysicalDevice const &physical) noexcept
{
    uint32_t count;
    vkGetPhysicalDeviceQueueFamilyProperties(physical, &count, nullptr);

    std::vector<VkQueueFamilyProperties> properties(count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical, &count, properties.data());

    return properties;
}

uint32_t select_queue_index(std::span<VkQueueFamilyProperties> properties) noexcept
{
    uint32_t index = 0;
    for (uint32_t i = 0; i < properties.size(); i++)
    {
        if (properties[i].queueCount > 0 && (properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
        {
            index = i;
            return index;
        }
    }

    return index;
}

void submit_queue(VkQueue const &queue, VkCommandBuffer const &cb, VkFence const &fence, VkSemaphore const &acquire,
                  VkSemaphore const &release)
{
    VkPipelineStageFlags mask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submit_info = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &acquire,
        .pWaitDstStageMask = &mask,
        .commandBufferCount = 1,
        .pCommandBuffers = &cb,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &release,
    };

    vkQueueSubmit(queue, 1, &submit_info, fence);
}

void queue_present(VulkanContext const &ctx, VkSemaphore const &release, uint32_t img_index)
{
    VkPresentInfoKHR info = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &release,
        .swapchainCount = 1,
        .pSwapchains = &ctx.m_VulkanSwapchain.m_Swapchain,
        .pImageIndices = &img_index,
    };
    vkQueuePresentKHR(ctx.m_VulkanDevice.m_Queue, &info);
}

} // namespace Device
} // namespace Utils
