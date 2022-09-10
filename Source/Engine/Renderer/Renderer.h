#pragma once

#include "Pipeline.h"
#include "VulkanTypes.h"

#include <Core/Asserts.h>
#include <FatalPCH.h>
#include <optional>
#include <utility>

namespace Utils
{
[[nodiscard]] static std::vector<VkLayerProperties> get_layer_properties() noexcept;

[[nodiscard]] bool validation_layers_supported(std::span<const char*> requested_layers) noexcept;

[[nodiscard]] VkInstance create_instance(const char* app_name) noexcept;
} // namespace Utils

namespace Device
{
[[nodiscard]] VulkanDevice create_device(VkInstance const& instance) noexcept;

[[nodiscard]] std::vector<VkPhysicalDevice> get_all_physical_devices(VkInstance const& instance) noexcept;

[[nodiscard]] std::vector<VkQueueFamilyProperties> get_queue_family_properties(
	VkPhysicalDevice const& physical) noexcept;

[[nodiscard]] uint32_t select_queue_index(std::span<VkQueueFamilyProperties> properties) noexcept;

void submit_queue(VkQueue const& queue, VkCommandBuffer const& cb, VkFence const& fence, VkSemaphore const& acquire,
	VkSemaphore const& release);

void queue_present(VulkanContext const& ctx, VkSemaphore const& release, uint32_t img_index);

VkFence initialize_fence(VkDevice const& device);

void handle_fences(VulkanContext const& ctx, VkFence const& fence, uint32_t& image_index, VkSemaphore const& acquire_semaphore);
} // namespace Device

namespace Swapchain
{
bool supports_present(VkPhysicalDevice const& physical, VkSurfaceKHR const& surface) noexcept;

std::vector<VkSurfaceFormatKHR> get_surface_formats(VkPhysicalDevice const& physical,
	VkSurfaceKHR const&                                                     surface) noexcept;

[[nodiscard]] VkSwapchainKHR create_vulkan_swapchain(VkPhysicalDevice const& physical, VkDevice const& device,
	VkSurfaceKHR const& surface) noexcept;

[[nodiscard]] VkRenderPass create_render_pass(VkPhysicalDevice const& physical, VkDevice const& device,
	VkSurfaceKHR const& surface) noexcept;

[[nodiscard]] VulkanSwapchain create_swapchain(VkPhysicalDevice const& physical, VkDevice const& device,
	VkSurfaceKHR const& surface, uint32_t width, uint32_t height) noexcept;

[[nodiscard]] std::vector<VkImageView> create_image_views(VkPhysicalDevice const& physical, VkDevice const& device,
	VkSwapchainKHR const& swapchain,
	VkSurfaceKHR const&   surface) noexcept;

[[nodiscard]] std::vector<VkFramebuffer> create_fbos(std::span<VkImageView> image_views, VkRenderPass const& rp,
	VkDevice const& device, uint32_t width, uint32_t height) noexcept;

std::pair<VkSemaphore, VkSemaphore> create_semaphores(VkDevice const& device) noexcept;

namespace RenderPass
{
void begin_render_pass(uint32_t img_index, VulkanContext const& ctx, VkCommandBuffer const& cb, uint32_t width,
	uint32_t height);
} // namespace RenderPass
} // namespace Swapchain

namespace Descriptor
{
[[nodiscard]] VkDescriptorSetLayout create_descriptor_layout(VkDevice const& device);

[[nodiscard]] VkDescriptorPool create_descriptor_pool(VkDevice const& device);

[[nodiscard]] VkDescriptorSet create_desriptor_set(VkDevice const& device, VkDescriptorPool const& pool,
	VkDescriptorSetLayout const& layout, VkBuffer const& buf,
	uint32_t size);

} // namespace Descriptor

VulkanContext create_context(std::string_view app_name, void* state, uint32_t width, uint32_t height);

namespace GLSL
{
[[nodiscard]] std::optional<std::vector<char>> get_file_contents(std::string&& path);

[[nodiscard]] VkShaderModule create_shader(VkDevice const& device, std::string path);
} // namespace GLSL

class Renderer
{
public:
	void initialize(std::string_view app_name, void* state, uint32_t width, uint32_t height);

	void render(uint32_t width, uint32_t height, VkBuffer const& vbo, VkBuffer const& ebo, uint64_t indices);

	VulkanContext const& get_context() const noexcept;

private:
	VulkanContext                       m_Context;
	std::pair<VkSemaphore, VkSemaphore> m_Semaphores;
	uint32_t                            m_QueueIndex;
	VkFence                             m_Fence;

	std::unique_ptr<CommandPool>   m_CommandPool;
	std::unique_ptr<CommandBuffer> m_CommandBuffer;

	VkShaderModule        m_VertexShader;
	VkShaderModule        m_FragmentShader;
	VkDescriptorSetLayout m_DescriptorLayout;
	PipelineBuilder       m_PipelineBuilder;
};
