#pragma once

#include <vector>
#include <vulkan/vulkan.h>

struct VulkanDevice
{
    VkPhysicalDevice m_PhysicalDevice;
    VkDevice m_Device;
};

struct VulkanSwapchain
{
    std::vector<VkImageView> m_ImageViews;
    std::vector<VkFramebuffer> m_Framebuffers;
    VkRenderPass m_RenderPass;
    VkSwapchainKHR m_Swapchain;
    VkFence m_Fence;
    // uint8_t m_FramesInFlight;
    // VkSurfaceFormatKHR m_SurfaceFormat;
};

struct VulkanContext
{
    VkInstance m_Instance;
    VkSurfaceKHR m_Surface;

    VulkanDevice m_VulkanDevice;
    VulkanSwapchain m_VulkanSwapchain;
};
