#include "nxpch.h"
#include "VulkanShaderCompiler.h"
#include "Next/Core/Utilities/StringUtils.h"

namespace Next {


	Ref<VulkanShader> VulkanShaderCompiler::Compile(const std::filesystem::path& shaderSourcePath)
	{
		const std::string& path = shaderSourcePath.string();
		Ref<VulkanShader> vkShader = Ref<VulkanShader>::Create(path);

		return vkShader;
	}
}