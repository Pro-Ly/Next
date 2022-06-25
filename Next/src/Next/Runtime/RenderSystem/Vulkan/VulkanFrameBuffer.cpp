#include "nxpch.h"
#include "VulkanFrameBuffer.h"

namespace Next {


	VulkanFrameBuffer::VulkanFrameBuffer(InitInfo initInfo)
		:m_InitInfo(initInfo)
	{
		VkFramebufferCreateInfo frameBufferCreateInfo = {};
		frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBufferCreateInfo.renderPass = m_InitInfo.vkRenderPass;
		frameBufferCreateInfo.attachmentCount = 1;
		frameBufferCreateInfo.pAttachments = &m_InitInfo.vkImageView;
		frameBufferCreateInfo.width = m_InitInfo.width;
		frameBufferCreateInfo.height = m_InitInfo.height;
		frameBufferCreateInfo.layers = 1;
		NX_CHECK_VKRESULT(vkCreateFramebuffer(m_InitInfo.vkDevice, &frameBufferCreateInfo, nullptr, &m_VkFrameBuffer));
	}

	VulkanFrameBuffer::~VulkanFrameBuffer()
	{
		vkDestroyFramebuffer(m_InitInfo.vkDevice, m_VkFrameBuffer, nullptr);
	}

}