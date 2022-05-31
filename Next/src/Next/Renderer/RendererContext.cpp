#include "nxpch.h"
#include "RendererContext.h"

#include "Next/Renderer/RendererAPI.h"
#include "Next/Core/Ref.h"
#include "Next/Platform/Vulkan/VulkanContext.h"

namespace Next {

	Ref<RendererContext> RendererContext::Create(GLFWwindow* windowHandle)
	{
		switch (RendererAPI::GetCurrentType())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::Vulkan:  return Ref<VulkanContext>::Create(windowHandle);
		}
		NX_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}