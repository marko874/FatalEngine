#pragma once

#include <vector>
#include <vulkan/vulkan.h>

enum class ShaderType
{
	Vertex = 0,
	Fragment
};

enum class RendererStream
{
	VertexRate = 0,
	InstanceRate,
};

class PipelineBuilder final
{
public:
	/**
	 * @brief Construct a new Pipeline Builder object
	 *
	 */
	PipelineBuilder();

	/**
	 * @brief Add the viewport state create info to the pipeline
	 *
	 * @param viewport_state The viewport state create info to add
	 * @return PipelineBuilder& to ensure chaining and the use of builder pattern
	 */
	[[nodiscard]] PipelineBuilder& init_viewport(uint32_t width, uint32_t height);

	/**
	 *
	 */
	[[nodiscard]] PipelineBuilder& add_shader(ShaderType st, VkShaderModule shader);

	/**
	 * @brief Adds an input attribute description. 
	 * 
	 * @return PipelineBuilder&
	 */
	[[nodiscard]] PipelineBuilder& add_attribute_description(uint32_t binding, uint32_t loc, VkFormat fmt, uint32_t offset) noexcept;

	[[nodiscard]] PipelineBuilder& add_input_binding(uint32_t stride, RendererStream rs) noexcept;

	[[nodiscard]] PipelineBuilder& add_descriptor_set_layout(VkDescriptorSetLayout const& layout);

	/**
	 * @brief Build the Vulkan pipeline
	 *
	 * @param ctx A lvalue-reference to the Vulkan context
	 */
	void build(VkDevice const& device, VkRenderPass const& rp);

	/**
	 * @brief PipelineBuilder dtor
	 */
	~PipelineBuilder() = default;

	/**
	 * @brief Get the Vulkan pipeline
	 *
	 * @return Vulkan pipeline
	 */
	[[nodiscard]] const VkPipeline& get() const noexcept;

	[[nodiscard]] const VkPipelineLayout& get_layout() const noexcept;

	/**
	 * @brief Bind the Vulkan pipeline
	 */
	void bind(VkCommandBuffer const& cmd_buf) noexcept;

private:
private:
	std::vector<VkPipelineShaderStageCreateInfo>   m_Stages;
	std::vector<VkVertexInputAttributeDescription> m_Attributes;
	VkPipeline                                     m_Pipeline;

	VkPipelineVertexInputStateCreateInfo   m_VertexInput;
	VkPipelineInputAssemblyStateCreateInfo m_Assembly;
	VkPipelineRasterizationStateCreateInfo m_Rasterization;
	VkPipelineMultisampleStateCreateInfo   m_Multisample;
	VkPipelineColorBlendAttachmentState    m_ColorAttachment;
	VkPipelineColorBlendStateCreateInfo    m_ColorBlend;
	VkVertexInputBindingDescription        m_InputBinding;

	VkPipelineViewportStateCreateInfo m_ViewportState;
	VkViewport                        m_Viewport;
	VkRect2D                          m_Scissors;

	VkPipelineLayoutCreateInfo m_LayoutInfo;
	VkPipelineLayout           m_Layout;

	VkGraphicsPipelineCreateInfo m_PipelineInfo;
};
