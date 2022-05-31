#include "hzpch.h"
#include "RendererContext.h"

#include "Next/Renderer/RendererAPI.h"
#include "Next/Core.h"
#include "Next/Platform/Vulkan/VulkanContext.h"

namespace Next {

	Ref<RendererContext> RendererContext::Create(GLFWwindow* windowHandle)
	{
		switch (RendererAPI::GetCurrentType())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::Vulkan:  return std::make_shared<VulkanContext>(windowHandle);
		}
		NX_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}