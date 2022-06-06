#pragma once
#include "Next/Core/Ref.h"
#include "Next/Platform/Vulkan/VulkanShader.h"
#include <filesystem>

namespace Next {

	class VulkanShaderCompiler : public RefCounted
	{
		static Ref<VulkanShader> Compile(const std::filesystem::path& shaderSourcePath);
	};
}


