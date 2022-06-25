#pragma once

#include "Runtime/RenderSystem/RendererAPI.h"
#include "Runtime/RenderSystem/Vulkan/VulkanContext.h"
#include "Runtime/RenderSystem/RenderSystem.h"

namespace Next {

	class VulkanRenderer : public RendererAPI
	{
	public:
		VulkanRenderer();
		virtual ~VulkanRenderer() override;

		inline static Ref<VulkanContext> GetContext() { return ((VulkanRenderer*)RenderSystem::GetRenderer())->m_Context; }

		virtual void Init()override;
		virtual void ShutDown()override;
		virtual void BeginFrame()override;
		virtual void drawFrame()override;
		virtual void EndFrame()override;

	private:
		Ref<VulkanContext> m_Context;
	};
}


