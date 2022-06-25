#include "nxpch.h"
#include "VulkanFrameBuffer.h"
#include "VulkanRenderer.h"

namespace Next {


	VulkanFrameBuffer::VulkanFrameBuffer(InitInfo initInfo)
		:m_InitInfo(initInfo)
	{
		auto VulkanDevice = VulkanRenderer::GetContext()->m_VulkanDevice;

		std::array<VkImageView, 3> attachments = {
			initInfo.colorImageView,
			initInfo.depthImageView,
			initInfo.swapChainImageView,
		};

		VkFramebufferCreateInfo frameBufferCreateInfo = {};
		frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBufferCreateInfo.renderPass = m_InitInfo.vkRenderPass;
		frameBufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());;
		frameBufferCreateInfo.pAttachments = attachments.data();
		frameBufferCreateInfo.width = m_InitInfo.width;
		frameBufferCreateInfo.height = m_InitInfo.height;
		frameBufferCreateInfo.layers = 1;
		NX_CHECK_VKRESULT(vkCreateFramebuffer(VulkanDevice->GetVkDevice(), &frameBufferCreateInfo, nullptr, &m_VkFrameBuffer));
	}

	VulkanFrameBuffer::~VulkanFrameBuffer()
	{
		auto VulkanDevice = VulkanRenderer::GetContext()->m_VulkanDevice;

		vkDestroyFramebuffer(VulkanDevice->GetVkDevice(), m_VkFrameBuffer, nullptr);
	}

}