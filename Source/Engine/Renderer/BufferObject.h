#pragma once

#include <Core/Asserts.h>
#include <Core/Logger.h>
#include <vulkan/vulkan.h>

enum class BufferType
{
	Vertex = 0,
	Index,
	Uniform
};

template<typename T, BufferType B>
class BufferObject
{
public:
	BufferObject() = default;

	BufferObject(VkDevice const& device, VkPhysicalDevice const& physical, uint64_t buffer_size)
		: m_BufferSize(buffer_size)
	{
		VkBufferUsageFlags flags = 0;
		switch(B)
		{
			case BufferType::Vertex:
				flags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
				break;
			case BufferType::Index:
				flags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
				break;
			case BufferType::Uniform:
				flags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
				break;
			default:
				break;
		}

		VkBufferCreateInfo info = {
			.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size        = m_BufferSize,
			.usage       = flags,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE
		};

		Assert::fatal_vk_assert(vkCreateBuffer(device, &info, nullptr, &m_Buffer));
		vkGetBufferMemoryRequirements(device, m_Buffer, &m_Requirements);

		m_MemoryIndex = find_memory_type(physical);

		VkMemoryAllocateInfo alloc_info = {
			.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.allocationSize  = m_Requirements.size,
			.memoryTypeIndex = m_MemoryIndex
		};

		Assert::fatal_vk_assert(vkAllocateMemory(device, &alloc_info, nullptr, &m_Memory));
		vkBindBufferMemory(device, m_Buffer, m_Memory, 0);

		Logger::log<Logger::Level::Info>("BufferObject ctor call.");
	}

	void map_memory(VkDevice const& device, T* data)
	{
		void* pp_data;
		Assert::fatal_vk_assert(vkMapMemory(device, m_Memory, 0, m_BufferSize, 0, &pp_data));

		std::memcpy(pp_data, data, m_BufferSize);
		vkUnmapMemory(device, m_Memory);
	}

	const VkBuffer& get_buffer() const noexcept
	{
		return m_Buffer;
	}

	const VkDeviceMemory& get_memory() const noexcept
	{
		return m_Memory;
	}

	[[nodiscard]] uint32_t find_memory_type(VkPhysicalDevice const& physical) noexcept
	{
		VkPhysicalDeviceMemoryProperties properties = {};
		vkGetPhysicalDeviceMemoryProperties(physical, &properties);

		const auto props = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		uint32_t   i     = 0;
		for(i; i < properties.memoryTypeCount; i++)
		{
			if((m_Requirements.memoryTypeBits & (1 << i)) != 0)
			{
				if((properties.memoryTypes[i].propertyFlags & props) == props)
				{
					return i;
				}
			}
		}
		return i;
	}

private:
	VkBuffer             m_Buffer;
	VkDeviceMemory       m_Memory;
	VkMemoryRequirements m_Requirements;

	uint32_t m_MemoryIndex;
	uint64_t m_BufferSize;
};
