#include "nxpch.h"
#include "VulkanShader.h"

namespace Next {

	VulkanShader::VulkanShader(VkDevice vkDevice, const std::string& vertFilePath, const std::string& fragFilePath)
		:m_VkDevice(vkDevice), Shader(vertFilePath, fragFilePath)
	{
		//vertShader
		if (!m_VertCode.empty())
		{
			VkShaderModule vertShaderModule = createShaderModule(m_VertCode);
			
			auto& vertCreateInfo = m_PipelinShaderStageCreateInfos.emplace_back();
			vertCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vertCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			vertCreateInfo.module = vertShaderModule;
			vertCreateInfo.pName = "main";
		}

		//fragShader
		if (!m_FragCode.empty())
		{
			VkShaderModule fragShaderModule = createShaderModule(m_FragCode);
			
			auto& fragCreateInfo = m_PipelinShaderStageCreateInfos.emplace_back();
			fragCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			fragCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			fragCreateInfo.module = fragShaderModule;
			fragCreateInfo.pName = "main";
		}
	}

	VulkanShader::~VulkanShader()
	{
		Release();
	}

	VkShaderModule VulkanShader::createShaderModule(std::vector<char>& code)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		NX_CHECK_VKRESULT(vkCreateShaderModule(m_VkDevice, &createInfo, nullptr, &shaderModule));

		return shaderModule;

	}

	void VulkanShader::Release()
	{
		for (const auto& createInfo : m_PipelinShaderStageCreateInfos)
		{
			if (createInfo.module)
				vkDestroyShaderModule(m_VkDevice, createInfo.module, nullptr);
		}
		m_PipelinShaderStageCreateInfos.clear();
	}

}