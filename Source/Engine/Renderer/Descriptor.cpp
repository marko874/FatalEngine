#include "Renderer.h"

#include <Core/Asserts.h>

using namespace Assert;
namespace Descriptor
{
VkDescriptorSetLayout create_descriptor_layout(VkDevice const& device)
{
	VkDescriptorSetLayoutBinding layout_binding = {
		.binding         = 0,
		.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1,
		.stageFlags      = VK_SHADER_STAGE_VERTEX_BIT,
	};

	VkDescriptorSetLayoutCreateInfo info = {
		.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.flags        = 0,
		.bindingCount = 1,
		.pBindings    = &layout_binding,
	};

	VkDescriptorSetLayout layout;
	fatal_vk_assert(vkCreateDescriptorSetLayout(device, &info, nullptr, &layout));

	return layout;
}

VkDescriptorPool create_descriptor_pool(VkDevice const& device)
{
	VkDescriptorPoolSize size = {
		.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1,
	};

	VkDescriptorPoolCreateInfo info = {
		.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.maxSets       = 1,
		.poolSizeCount = 1,
		.pPoolSizes    = &size,
	};

	VkDescriptorPool pool;
	fatal_vk_assert(vkCreateDescriptorPool(device, &info, nullptr, &pool));

	return pool;
}

VkDescriptorSet create_desriptor_set(VkDevice const& device, VkDescriptorPool const& pool,
	VkDescriptorSetLayout const& layout, VkBuffer const& buf, uint32_t size)
{
	VkDescriptorSetAllocateInfo alloc_info = {
		.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool     = pool,
		.descriptorSetCount = 1,
		.pSetLayouts        = &layout,
	};

	VkDescriptorSet set;
	fatal_vk_assert(vkAllocateDescriptorSets(device, &alloc_info, &set));

	VkDescriptorBufferInfo buffer_info = {
		.buffer = buf,
		.offset = 0,
		.range  = size,
	};

	VkWriteDescriptorSet write = {
		.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet          = set,
		.dstArrayElement = 0,
		.descriptorCount = 1,
		.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.pBufferInfo     = &buffer_info,
	};

	vkUpdateDescriptorSets(device, 1, &write, 0, nullptr);
	return set;
}
} // namespace Descriptor
