#include "nxpch.h"
#include "RendererAPI.h"
#include "Runtime/RenderSystem/Vulkan/VulkanRenderer.h"

namespace Next {


	RendererAPI* RendererAPI::Create(RendererAPIType apiType)
	{
		switch (apiType)
		{
			case Next::RendererAPIType::None:
				NX_CORE_ASSERT(true, "None API Type!");
				break;
			case Next::RendererAPIType::Vulkan:
				return new VulkanRenderer();
				break;
		}
		NX_CORE_ASSERT(true, "Unknown API Type!");
		return nullptr;
	}

}