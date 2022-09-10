#include "Renderer.h"

#include <Core/Asserts.h>
#include <Core/Logger.h>
#include <Core/PlatformLayer.h>
#include <Core/WindowsLayer.h>
#include <FatalPCH.h>
#include <Renderer/BufferObject.h>
#include <vulkan/vulkan.h>

using enum Logger::Level;
using namespace Assert;

VulkanContext create_context(std::string_view app_name, void* state, uint32_t width, uint32_t height)
{
	VulkanContext ctx = {
		.m_Instance     = Utils::create_instance(app_name.data()),
		.m_Surface      = Platform::create_vulkan_surface(ctx.m_Instance, state),
		.m_VulkanDevice = Device::create_device(ctx.m_Instance),
	};

	ctx.m_VulkanSwapchain = Swapchain::create_swapchain(
		ctx.m_VulkanDevice.m_PhysicalDevice, ctx.m_VulkanDevice.m_Device, ctx.m_Surface, width, height);

	Logger::log<Info>("Context created successfully.");
	return ctx;
}

void Renderer::initialize(std::string_view app_name, void* state, uint32_t width, uint32_t height)
{
	m_Context          = create_context(app_name, state, width, height);
	auto const& device = m_Context.m_VulkanDevice;

	m_Semaphores = Swapchain::create_semaphores(m_Context.m_VulkanDevice.m_Device);

	auto properties = Device::get_queue_family_properties(device.m_PhysicalDevice);
	m_QueueIndex    = Device::select_queue_index(properties);

	m_Fence = Device::initialize_fence(device.m_Device);

	m_CommandPool   = std::make_unique<CommandPool>(device.m_Device, m_QueueIndex);
	m_CommandBuffer = std::make_unique<CommandBuffer>(device.m_Device, m_CommandPool->get(), CommandBufferLevel::Primary);

	m_VertexShader   = GLSL::create_shader(device.m_Device, "../Shaders/out/Model.vert.spv");
	m_FragmentShader = GLSL::create_shader(device.m_Device, "../Shaders/out/Model.frag.spv");

	m_DescriptorLayout = Descriptor::create_descriptor_layout(device.m_Device);

	m_PipelineBuilder = PipelineBuilder()
	                        .add_shader(ShaderType::Vertex, m_VertexShader)
	                        .add_shader(ShaderType::Fragment, m_FragmentShader)
	                        .init_viewport(width, height)
	                        .add_input_binding(3 * sizeof(float), RendererStream::VertexRate)
	                        .add_attribute_description(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0)
	                        //.add_attribute_description(0, 1, VK_FORMAT_R32G32_SFLOAT, 2 * sizeof(float))
	                        .add_descriptor_set_layout(m_DescriptorLayout);

	m_PipelineBuilder.build(device.m_Device, m_Context.m_VulkanSwapchain.m_RenderPass);

	Logger::log<Logger::Level::Info>("Renderer initialized successfully.");
}

VulkanContext const& Renderer::get_context() const noexcept
{
	return m_Context;
}
#pragma warning(disable : 4100)
void Renderer::render(uint32_t width, uint32_t height, VkBuffer const& vbo, VkBuffer const& ebo, uint64_t indices)
{
	uint32_t img_index = 0;
	Device::handle_fences(m_Context, m_Fence, img_index, m_Semaphores.first);
	m_CommandPool->reset(m_Context.m_VulkanDevice.m_Device);

	m_CommandBuffer->begin();
	auto const& cb = m_CommandBuffer->get_buffer();

	Swapchain::RenderPass::begin_render_pass(img_index, m_Context, cb, width, height);
	m_PipelineBuilder.bind(cb);

	VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(cb, 0, 1, &vbo, &offset);
	vkCmdBindIndexBuffer(cb, ebo, 0, VK_INDEX_TYPE_UINT16);
	vkCmdDrawIndexed(cb, static_cast<uint32_t>(indices), 1, 0, 0, 0);

	vkCmdEndRenderPass(cb);
	m_CommandBuffer->end();

	Device::submit_queue(m_Context.m_VulkanDevice.m_Queue, cb, m_Fence, m_Semaphores.first, m_Semaphores.second);
	Device::queue_present(m_Context, m_Semaphores.second, img_index);
}
