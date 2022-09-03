#include "Renderer.h"

#include <fstream>

namespace GLSL
{
std::optional<std::vector<char>> get_file_contents(std::string&& path)
{
	std::ifstream file(path, std::ios::ate | std::ios_base::binary);
	auto          file_size = file.tellg();
	file.seekg(0);

	std::vector<char> source(file_size);
	file.read(source.data(), file_size);
	file.close();

	if(source.empty())
	{
		return {};
	}
	else
	{
		return source;
	}
}

VkShaderModule create_shader(VkDevice device, std::string path)
{
	auto src = get_file_contents(std::move(path));

	VkShaderModuleCreateInfo create_info = {
		.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = src.value().size(),
		.pCode    = reinterpret_cast<const uint32_t*>(src.value().data()),
	};

	VkShaderModule shader;
	vkCreateShaderModule(device, &create_info, nullptr, &shader);

	return shader;
}
} // namespace GLSL
