#pragma once

#include "Runtime/RenderSystem/Shader.h"

#include <string>
#include <vector>
#include <filesystem>

namespace Next {

	class VulkanShader : public Shader
	{
	public:
		VulkanShader() = default;
		VulkanShader(const std::string& filePath);
		inline std::vector<char> GetCode() const { return m_Code; }

		//static VkShaderModule CreateShaderModule(const std::vector<char>& code);
	private:
		std::filesystem::path m_Path;
		std::string m_Name;
		std::vector<char> m_Code;
		//VkShaderModule m_ShaderModule;
	private:
		std::vector<char> ReadFile(const std::string& filePath);
	};
}


