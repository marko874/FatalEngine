#include "Model.h"

#pragma warning(push, 0)
#define CGLTF_IMPLEMENTATION
#include <cgltf.h>
#pragma warning(pop)

void Model::create_model(std::string_view path, VkDevice const& device, VkPhysicalDevice const& physical)
{
	cgltf_options options = {};
	cgltf_data*   data    = 0;
	cgltf_result  error   = cgltf_parse_file(&options, path.data(), &data);

	if(error == cgltf_result_success)
	{
		error = cgltf_load_buffers(&options, data, "../Assets");
		if(error == cgltf_result_success)
		{
			assert(data->meshes_count == 1);
			assert(data->meshes[0].primitives_count == 1);
			assert(data->meshes[0].primitives[0].attributes_count > 0);
			assert(data->meshes[0].primitives[0].attributes[0].type == cgltf_attribute_type_position);
			assert(data->meshes[0].primitives[0].attributes[0].data->stride == sizeof(float) * 3);
			assert(data->meshes[0].primitives[0].indices->component_type == cgltf_component_type_r_16u);
			assert(data->meshes[0].primitives[0].indices->stride == sizeof(uint16_t));

			uint8_t* buffer_base = static_cast<uint8_t*>(data->meshes[0].primitives[0].indices->buffer_view->buffer->data);
			uint64_t ebo_size    = data->meshes[0].primitives[0].indices->buffer_view->size;
			void*    ebo_data    = buffer_base + data->meshes[0].primitives[0].indices->buffer_view->offset;

			m_IndexBuffer = new BufferObject<uint16_t, BufferType::Index>(device, physical, ebo_size);
			m_IndexBuffer->map_memory(device, static_cast<uint16_t*>(ebo_data));
			m_NumIndices = data->meshes[0].primitives[0].indices->count;

			buffer_base       = static_cast<uint8_t*>(data->meshes[0].primitives[0].attributes[0].data->buffer_view->buffer->data);
			uint64_t vbo_size = data->meshes[0].primitives[0].attributes[0].data->buffer_view->size;
			void*    vbo_data = buffer_base + data->meshes[0].primitives[0].attributes[0].data->buffer_view->offset;

			m_VertexBuffer = new BufferObject<float, BufferType::Vertex>(device, physical, vbo_size);
			m_VertexBuffer->map_memory(device, static_cast<float*>(vbo_data));
		}
		cgltf_free(data);
	}
}

uint64_t const& Model::get_num_indices() const noexcept
{
	return m_NumIndices;
}

BufferObject<float, BufferType::Vertex> const& Model::get_vertex_buffer_object() const noexcept
{
	return *m_VertexBuffer;
}

BufferObject<uint16_t, BufferType::Index> const& Model::get_index_buffer_object() const noexcept
{
	return *m_IndexBuffer;
}

Model::~Model()
{
	delete m_IndexBuffer;
	delete m_VertexBuffer;
}
