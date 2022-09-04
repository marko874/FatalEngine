#include "Pipeline.h"

#include <Core/Asserts.h>
using namespace Assert;

PipelineBuilder::PipelineBuilder()
	: m_VertexInput{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
	}
	, m_Rasterization{
		.sType     = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
		.lineWidth = 1.0f,
	}
	, m_Multisample{
		.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
	}
{
	m_Assembly = {
		.sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
	};

	m_ColorAttachment = {
		.blendEnable    = VK_FALSE,
		.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
	};

	m_ColorBlend = {
		.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.attachmentCount = 1,
		.pAttachments    = &m_ColorAttachment
	};
}

PipelineBuilder& PipelineBuilder::init_viewport(uint32_t width, uint32_t height)
{
	m_Viewport = { 0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height) };
	m_Scissors = { { 0, 0 }, { width, height } };

	m_ViewportState = {
		.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.pViewports    = &m_Viewport,

		.scissorCount = 1,
		.pScissors    = &m_Scissors
	};

	return *this;
}

PipelineBuilder& PipelineBuilder::add_shader(ShaderType st, VkShaderModule shader)
{
	VkShaderStageFlagBits stage = {};
	switch(st)
	{
		case ShaderType::Vertex:
			stage = VK_SHADER_STAGE_VERTEX_BIT;
			break;
		case ShaderType::Fragment:
			stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			break;
	}

	VkPipelineShaderStageCreateInfo info = {
		.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage  = stage,
		.module = shader,
		.pName  = "main"
	};

	m_Stages.emplace_back(info);
	return *this;
}

PipelineBuilder& PipelineBuilder::add_attribute_description(uint32_t binding, uint32_t loc, VkFormat fmt, uint32_t offset) noexcept
{
	VkVertexInputAttributeDescription attrib = {
		.location = loc,
		.binding  = binding,
		.format   = fmt,
		.offset   = offset
	};

	m_Attributes.emplace_back(std::move(attrib));
	return *this;
}

PipelineBuilder& PipelineBuilder::add_input_binding(uint32_t stride, RendererStream rs = RendererStream::VertexRate) noexcept
{
	VkVertexInputRate rate = {};
	switch(rs)
	{
		case RendererStream::VertexRate:
			rate = VK_VERTEX_INPUT_RATE_VERTEX;
			break;
		case RendererStream::InstanceRate:
			rate = VK_VERTEX_INPUT_RATE_INSTANCE;
			break;
		default:
			break;
	}

	m_InputBinding = {
		.binding   = 0,
		.stride    = stride,
		.inputRate = rate,
	};

	m_VertexInput.vertexBindingDescriptionCount = 1;
	m_VertexInput.pVertexBindingDescriptions    = &m_InputBinding;

	return *this;
}

PipelineBuilder& PipelineBuilder::add_descriptor_set_layout(VkDescriptorSetLayout const& layout)
{
	m_LayoutInfo = {
		.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 1,
		.pSetLayouts    = &layout,
	};

	return *this;
}

void PipelineBuilder::build(VkDevice const& device, VkRenderPass const& rp)
{
	m_VertexInput.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_Attributes.size());
	m_VertexInput.pVertexAttributeDescriptions    = m_Attributes.data();

	fatal_vk_assert(vkCreatePipelineLayout(device, &m_LayoutInfo, 0, &m_Layout));

	m_PipelineInfo = {
		.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.stageCount          = static_cast<unsigned>(m_Stages.size()),
		.pStages             = m_Stages.data(),
		.pVertexInputState   = &m_VertexInput,
		.pInputAssemblyState = &m_Assembly,
		.pViewportState      = &m_ViewportState,
		.pRasterizationState = &m_Rasterization,
		.pMultisampleState   = &m_Multisample,
		.pColorBlendState    = &m_ColorBlend,
		.layout              = m_Layout,
		.renderPass          = rp,
		.subpass             = 0,
	};

	fatal_vk_assert(vkCreateGraphicsPipelines(device, nullptr, 1, &m_PipelineInfo, nullptr, &m_Pipeline));
}

VkPipeline const& PipelineBuilder::get() const noexcept
{
	return m_Pipeline;
}

VkPipelineLayout const& PipelineBuilder::get_layout() const noexcept
{
	return m_Layout;
}

void PipelineBuilder::bind(VkCommandBuffer const& cmd_buf) noexcept
{
	vkCmdBindPipeline(cmd_buf, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline);
}
