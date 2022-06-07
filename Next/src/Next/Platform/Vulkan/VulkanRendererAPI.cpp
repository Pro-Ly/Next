#include "nxpch.h"
#include "VulkanRendererAPI.h"
#include "Next/Platform/Windows/WindowsWindow.h"
#include "VulkanRenderCommandBuffer.h"
#include "VulkanPipeline.h"

#include <vulkan/vulkan.h>

namespace Next {

	VulkanRendererAPI::VulkanRendererAPI()
	{

	}

	VulkanRendererAPI::~VulkanRendererAPI()
	{

	}

	void VulkanRendererAPI::Init()
	{

	}

	void VulkanRendererAPI::Shutdown()
	{

	}

	void VulkanRendererAPI::BeginRenderPass(const Ref<RenderCommandBuffer>& commandBuffer)
	{
		VulkanSwapChain& swapChain = WindowsWindow::Get().GetSwapChain();
		auto vkCommandBuffer = commandBuffer.As<VulkanRenderCommandBuffer>();
;		auto renderPass = swapChain.GetRenderPass();
		auto swapChainFramebuffers = swapChain.GetSwapChainFramebuffers();
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = swapChainFramebuffers[0];

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChain.GetExtend2D();

		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(vkCommandBuffer->GetCommandBuffer(0), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	}

	void VulkanRendererAPI::EndRenderPass(const Ref<RenderCommandBuffer>& commandBuffer)
	{
		auto vkCommandBuffer = commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(0);
		vkCmdEndRenderPass(vkCommandBuffer);
		if (vkEndCommandBuffer(vkCommandBuffer) != VK_SUCCESS) {
			NX_CORE_ASSERT(false,"failed to record command buffer!");
		}
	}

	void VulkanRendererAPI::Draw(const Ref<RenderCommandBuffer>& commandBuffer, const Ref<Pipeline>& pipeline)
	{
		auto vkCommandBuffer = commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(0);
		auto graphicsPipeline = pipeline.As<VulkanPipeline>()->GetGraphicsPipeline();
		vkCmdBindPipeline(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
		vkCmdDraw(vkCommandBuffer, 3, 1, 0, 0);
	}

}

