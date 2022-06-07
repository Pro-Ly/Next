#pragma once

#include "RenderCommandBuffer.h"
namespace Next {

	enum class RendererAPIType
	{
		None,
		Vulkan,
	};

	class RendererAPI
	{
	public:
		virtual ~RendererAPI() = default;
		inline static RendererAPIType GetCurrentType() { return s_CurRendererAPIType; }
		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void BeginRenderPass(const Ref<RenderCommandBuffer>& commandBuffer) = 0;
		virtual void EndRenderPass(const Ref<RenderCommandBuffer>& commandBuffer) = 0;
	private:
		inline static RendererAPIType s_CurRendererAPIType = RendererAPIType::Vulkan;//Only for now
	};
}