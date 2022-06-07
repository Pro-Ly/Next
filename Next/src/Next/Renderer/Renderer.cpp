#include "nxpch.h"
#include "Renderer.h"
#include "Next/Platform/Vulkan/VulkanRendererAPI.h"

namespace Next {


	RendererAPI* Renderer::InitRendererAPI()
	{
		switch (RendererAPI::GetCurrentType())
		{
			case RendererAPIType::None:
				NX_CORE_ASSERT(false, "Cannot Init None Renderer API !!!");
				break;
			case RendererAPIType::Vulkan:
				return new VulkanRendererAPI();

		}
		NX_CORE_ASSERT(false, "Unknown Renderer API !!!");
		return nullptr;
	}

	void Renderer::Init()
	{
		s_RendererAPI = InitRendererAPI();
		s_RendererAPI->Init();

	}

	void Renderer::Shutdown()
	{
		if (s_RendererAPI)
		{
			s_RendererAPI->Shutdown();
			delete s_RendererAPI;
		}
	}

	void Renderer::BeginRenderPass(const Ref<RenderCommandBuffer>& commandBuffer)
	{
		s_RendererAPI->BeginRenderPass(commandBuffer);
	}

	RenderCommandQueue& Renderer::GetRenderCommandQueue()
	{
		return *s_CommandQueue;
	}

}