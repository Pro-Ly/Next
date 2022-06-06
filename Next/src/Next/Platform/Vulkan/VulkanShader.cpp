#include "nxpch.h"
#include "VulkanShader.h"
#include "Next/Core/Utilities/StringUtils.h"
#include "Next/Renderer/Renderer.h"
#include "VulkanContext.h"
#include "Next/Core/Core.h"
#include "Next/Core/Ref.h"

#include <fstream>
namespace Next {


	VulkanShader::VulkanShader(const std::string& filePath)
		:m_Path(filePath)
	{
		m_Name = StringUtils::GetFileName(filePath);

		m_Code = ReadFile(filePath);


	}

	std::vector<char> VulkanShader::ReadFile(const std::string& filePath)
	{
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			NX_CORE_ASSERT(false,"failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	VkShaderModule VulkanShader::CreateShaderModule(const std::vector<char>& code)
	{
		auto device = VulkanContext::GetCurrentDevice()->GetLogicalDevice();
		VkShaderModule shaderModule;
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			NX_CORE_ASSERT(false,"failed to create shader module!");
		}
		return shaderModule;
	}

}