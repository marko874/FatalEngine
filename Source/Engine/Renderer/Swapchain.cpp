#include "Renderer.h"

namespace Utils
{
namespace Swapchain
{
// TODO: rewrite in a more OOP-way.

bool supports_present(VkPhysicalDevice const &physical, VkSurfaceKHR const &surface) noexcept
{
    VkBool32 supports = 0;
    vkGetPhysicalDeviceSurfaceSupportKHR(physical, 0, surface, &supports);

    return supports;
}

std::vector<VkSurfaceFormatKHR> get_surface_formats(VkPhysicalDevice const &physical,
                                                    VkSurfaceKHR const &surface) noexcept
{
    uint32_t count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical, surface, &count, nullptr);

    std::vector<VkSurfaceFormatKHR> formats(count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical, surface, &count, formats.data());

    return formats;
}

VkSwapchainKHR create_vulkan_swapchain(VkPhysicalDevice const &physical, VkDevice const &device,
                                       VkSurfaceKHR const &surface) noexcept
{
    assert(supports_present(physical, surface));

    const auto formats = get_surface_formats(physical, surface);
    const auto default_format = formats[0];

    VkSurfaceCapabilitiesKHR caps;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical, surface, &caps);

    VkSwapchainCreateInfoKHR create_info = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface,
        .minImageCount = 3,
        .imageFormat = default_format.format,
        .imageColorSpace = default_format.colorSpace,
        .imageExtent = caps.currentExtent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_FIFO_KHR,
    };

    VkSwapchainKHR swapchain;
    vkCreateSwapchainKHR(device, &create_info, nullptr, &swapchain);

    return swapchain;
}

VkRenderPass create_render_pass(VkPhysicalDevice const &physical, VkDevice const &device,
                                VkSurfaceKHR const &surface) noexcept
{
    auto fmt = get_surface_formats(physical, surface)[0].format;

    VkAttachmentDescription attachment = {
        .format = fmt,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    };

    VkAttachmentReference attachment_ref = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    VkSubpassDescription subpass = {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments = &attachment_ref,
    };

    VkRenderPassCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = 1,
        .pAttachments = &attachment,
        .subpassCount = 1,
        .pSubpasses = &subpass,
    };

    VkRenderPass render_pass;
    vkCreateRenderPass(device, &create_info, nullptr, &render_pass);

    return render_pass;
}

std::vector<VkImageView> create_image_views(VkPhysicalDevice const &physical, VkDevice const &device,
                                            VkSwapchainKHR const &swapchain, VkSurfaceKHR const &surface) noexcept
{
    uint32_t count = 0;
    vkGetSwapchainImagesKHR(device, swapchain, &count, nullptr);
    std::vector<VkImage> images(count);

    vkGetSwapchainImagesKHR(device, swapchain, &count, images.data());
    std::vector<VkImageView> image_views(count);

    auto fmt = get_surface_formats(physical, surface)[0].format;

    for (uint32_t i = 0; i < count; i++)
    {
        VkImageViewCreateInfo create_info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = images[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = fmt,
            .components = {},
            .subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1},
        };

        vkCreateImageView(device, &create_info, nullptr, &image_views[i]);
    }

    return image_views;
}

[[nodiscard]] std::vector<VkFramebuffer> create_fbos(std::span<VkImageView> image_views, VkRenderPass const &rp,
                                                     VkDevice const &device, uint32_t width, uint32_t height) noexcept
{
    auto sz = image_views.size();

    std::vector<VkFramebuffer> fbos(sz);

    for (uint32_t i = 0; i < sz; i++)
    {
        VkFramebufferCreateInfo info = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = rp,
            .attachmentCount = 1,
            .pAttachments = &image_views[i],
            .width = width,
            .height = height,
            .layers = 1,
        };
        vkCreateFramebuffer(device, &info, nullptr, &fbos[i]);
    }

    return fbos;
}

VulkanSwapchain create_swapchain(VkPhysicalDevice const &physical, VkDevice const &device, VkSurfaceKHR const &surface,
                                 uint32_t width, uint32_t height) noexcept
{
    VulkanSwapchain swapchain = {};
    swapchain.m_Swapchain = create_vulkan_swapchain(physical, device, surface);
    swapchain.m_RenderPass = create_render_pass(physical, device, surface);
    swapchain.m_ImageViews = create_image_views(physical, device, swapchain.m_Swapchain, surface);
    swapchain.m_Framebuffers = create_fbos(swapchain.m_ImageViews, swapchain.m_RenderPass, device, width, height);

    return swapchain;
}

std::pair<VkSemaphore, VkSemaphore> create_semaphores(VkDevice const &device) noexcept
{
    VkSemaphoreCreateInfo info = {.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

    VkSemaphore acquire;
    VkSemaphore release;
    vkCreateSemaphore(device, &info, nullptr, &acquire);
    vkCreateSemaphore(device, &info, nullptr, &release);

    return {acquire, release};
}

} // namespace Swapchain
} // namespace Utils
