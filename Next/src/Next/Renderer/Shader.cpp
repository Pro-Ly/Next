#include "nxpch.h"
#include "Shader.h"
#include "RendererAPI.h"
#include "Next/Platform/Vulkan/VulkanShader.h"


namespace Next {

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		Ref<Shader> result = nullptr;

		switch (RendererAPI::GetCurrentType())
		{
			case RendererAPIType::None: return nullptr;
			case RendererAPIType::Vulkan:
				result = Ref<VulkanShader>::Create(filepath);
				break;
		}
		return result;
	}

	Ref<Shader> Shader::CreateFromString(const std::string& shaderSrc)
	{
		return nullptr;
	}

}