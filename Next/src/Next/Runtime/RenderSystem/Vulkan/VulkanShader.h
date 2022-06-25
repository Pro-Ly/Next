#pragma once

#include "Runtime/RenderSystem/Shader.h"
#include "VulkanUtils.h"

namespace Next {

	class VulkanShader : public Shader
	{
	public:
		VulkanShader(VkDevice vkDevice,const std::string& vertFilePath,const std::string& fragFilePath);
		virtual ~VulkanShader() override;

		inline const std::vector<VkPipelineShaderStageCreateInfo> GetShaderStageCreateInfos() const {
			return m_PipelinShaderStageCreateInfos;
		};

		void Release();
	private:
		VkDevice m_VkDevice;

		std::vector<VkPipelineShaderStageCreateInfo> m_PipelinShaderStageCreateInfos;

	private:
		VkShaderModule createShaderModule(std::vector<char>& code);
	};
}


