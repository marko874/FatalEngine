#include "VulkanTypes.h"

CommandPool::CommandPool(VkDevice const& device, uint32_t index)
{
	VkCommandPoolCreateInfo info = {
		.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags            = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
		.queueFamilyIndex = index,
	};
	vkCreateCommandPool(device, &info, nullptr, &m_Pool);
}

const VkCommandPool& CommandPool::get() const noexcept
{
	return m_Pool;
}

void CommandPool::reset(VkDevice const& device) noexcept
{
	vkResetCommandPool(device, m_Pool, 0);
}

CommandBuffer::CommandBuffer(VkDevice const& device, VkCommandPool const& pool,
	CommandBufferLevel lvl = CommandBufferLevel::Primary)
{
	VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	switch(lvl)
	{
		case CommandBufferLevel::Primary:
			level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			break;
		/* ... */
		default:
			break;
	}

	VkCommandBufferAllocateInfo alloc_info = {
		.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool        = pool,
		.level              = level,
		.commandBufferCount = 1,
	};

	vkAllocateCommandBuffers(device, &alloc_info, &m_Buffer);
	set_begin_info();
}

const VkCommandBuffer& CommandBuffer::get_buffer() const noexcept
{
	return m_Buffer;
}

void CommandBuffer::begin() noexcept
{
	vkBeginCommandBuffer(m_Buffer, &m_BeginInfo);
}

void CommandBuffer::end() noexcept
{
	vkEndCommandBuffer(m_Buffer);
}

void CommandBuffer::set_begin_info()
{
	m_BeginInfo = {
		.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.pNext            = nullptr,
		.flags            = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
		.pInheritanceInfo = nullptr,
	};
}
