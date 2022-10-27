#include "Editor.h"

#include <Core/Asserts.h>
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_win32.h>
#include <span>

namespace Editor
{

void init_ui(VkDevice const& device, VkPhysicalDevice const& physical, void* hwnd, VkQueue const& queue, VkRenderPass const& render_pass, VkInstance const& instance) noexcept
{
	VkDescriptorPoolSize pool_sizes[] = {
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};

	VkDescriptorPoolCreateInfo pool_info = {
		.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
		.maxSets       = 1000,
		.poolSizeCount = static_cast<uint32_t>(std::size(pool_sizes)),
		.pPoolSizes    = pool_sizes,
	};

	VkDescriptorPool imgui_pool;
	Assert::fatal_vk_assert(vkCreateDescriptorPool(device, &pool_info, nullptr, &imgui_pool));

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io                = ImGui::GetIO();
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hwnd);

	ImGui_ImplVulkan_InitInfo init_info = {
		.Instance       = instance,
		.PhysicalDevice = physical,
		.Device         = device,
		.Queue          = queue,
		.DescriptorPool = imgui_pool,
		.MinImageCount  = 3,
		.ImageCount     = 3,
		.MSAASamples    = VK_SAMPLE_COUNT_1_BIT,
	};

	ImGui_ImplVulkan_Init(&init_info, render_pass);
}
} // namespace Editor
