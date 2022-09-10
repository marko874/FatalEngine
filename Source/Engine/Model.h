#pragma once

#include <Renderer/BufferObject.h>

class Model
{
public:
	Model() = default;
	void create_model(std::string_view path, VkDevice const& device, VkPhysicalDevice const& physical);

	~Model();

	Model(Model const&)            = delete;
	Model& operator=(Model const&) = delete;
	Model(Model&&)                 = delete;
	Model& operator=(Model&&)      = delete;

	uint64_t const&                           get_num_indices() const noexcept;
	BufferObject<float, BufferType::Vertex> const&   get_vertex_buffer_object() const noexcept;
	BufferObject<uint16_t, BufferType::Index> const& get_index_buffer_object() const noexcept;

private:
	BufferObject<float, BufferType::Vertex>*   m_VertexBuffer;
	BufferObject<uint16_t, BufferType::Index>* m_IndexBuffer;
	uint64_t                                   m_NumIndices;
};
