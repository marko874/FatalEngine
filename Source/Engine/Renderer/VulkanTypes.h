#pragma once

#include <vector>
#include <vulkan/vulkan.h>

struct VulkanDevice
{
	VkPhysicalDevice m_PhysicalDevice;
	VkDevice         m_Device;
	VkQueue          m_Queue;
};

struct VulkanSwapchain
{
	std::vector<VkImageView>   m_ImageViews;
	std::vector<VkFramebuffer> m_Framebuffers;
	VkRenderPass               m_RenderPass;
	VkSwapchainKHR             m_Swapchain;
	VkFence                    m_Fence;
	// uint8_t m_FramesInFlight;
	// VkSurfaceFormatKHR m_SurfaceFormat;
};

class CommandPool
{
public:
	explicit CommandPool(VkDevice const& device, uint32_t idx);

	~CommandPool() = default;

	[[nodiscard]] const VkCommandPool& get() const noexcept;

	void reset(VkDevice const& device) noexcept;

private:
	VkCommandPool m_Pool;
};

enum class CommandBufferLevel
{
	Primary = 0,
};

class CommandBuffer
{
public:
	explicit CommandBuffer(VkDevice const& device, VkCommandPool const& pool, CommandBufferLevel lvl);

	~CommandBuffer() = default;

	[[nodiscard]] const VkCommandBuffer& get_buffer() const noexcept;

	void begin() noexcept;
	void end() noexcept;

private:
	void set_begin_info();

private:
	VkCommandBuffer          m_Buffer;
	VkCommandBufferBeginInfo m_BeginInfo;
};

struct VulkanContext
{
	VkInstance   m_Instance;
	VkSurfaceKHR m_Surface;

	VulkanDevice    m_VulkanDevice;
	VulkanSwapchain m_VulkanSwapchain;
};
