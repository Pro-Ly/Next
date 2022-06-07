#include "nxpch.h"
#include "RenderCommandBuffer.h"
#include "RendererAPI.h"
#include "Next/Platform/Vulkan/VulkanRenderCommandBuffer.h"

namespace Next {

	Ref<RenderCommandBuffer> RenderCommandBuffer::Create()
	{
		switch (RendererAPI::GetCurrentType())
		{
			case RendererAPIType::None:
				NX_CORE_ASSERT(false, "Cannot Create None Renderer API !!!");
				break;
			case RendererAPIType::Vulkan:
				return Ref<VulkanRenderCommandBuffer>::Create();
				break;
		}
		NX_CORE_ASSERT(false, "Unknown Renderer API !!!");
		return nullptr;
	}

}