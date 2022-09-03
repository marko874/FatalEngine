#include "Renderer.h"

#include <Core/Asserts.h>
#include <Core/Logger.h>
#include <Core/PlatformLayer.h>
#include <Core/WindowsLayer.h>
#include <array>
#include <span>
#include <vulkan/vulkan.h>

using enum Logger::Level;
using namespace Assert;

VulkanContext create_context(std::string_view app_name, void* state, uint32_t width, uint32_t height)
{
	VulkanContext ctx = {
		.m_Instance     = Utils::create_instance(app_name.data()),
		.m_Surface      = Platform::create_vulkan_surface(ctx.m_Instance, state),
		.m_VulkanDevice = Utils::Device::create_device(ctx.m_Instance),
	};

	ctx.m_VulkanSwapchain = Utils::Swapchain::create_swapchain(
		ctx.m_VulkanDevice.m_PhysicalDevice, ctx.m_VulkanDevice.m_Device, ctx.m_Surface, width, height);

	Logger::log<Info>("Context created successfully.");
	return ctx;
}
