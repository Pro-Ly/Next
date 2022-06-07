#pragma once

#include "Next/Renderer/RendererAPI.h"
#include "Next/Renderer/RendererContext.h"
#include "Next/Renderer/RenderCommandBuffer.h"
#include "Next/Renderer/Pipeline.h"

namespace Next {

	class VulkanRendererAPI : public RendererAPI
	{
	public:
		VulkanRendererAPI();
		virtual ~VulkanRendererAPI() override;

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void BeginRenderPass(const Ref<RenderCommandBuffer>& commandBuffer) override;
		virtual void EndRenderPass(const Ref<RenderCommandBuffer>& commandBuffer) override;

		void Draw(const Ref<RenderCommandBuffer>& commandBuffer, const Ref<Pipeline>& pipeline);
	private:
		
	};


}


