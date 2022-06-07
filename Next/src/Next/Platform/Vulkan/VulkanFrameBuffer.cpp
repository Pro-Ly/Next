#include "nxpch.h"
#include "VulkanFrameBuffer.h"
#include <vulkan/vulkan.h>

namespace Next {


	VulkanFrameBuffer::VulkanFrameBuffer()
	{
		//VkFramebufferCreateInfo framebufferInfo{};
		//framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		//framebufferInfo.renderPass = renderPass;
		//framebufferInfo.attachmentCount = 1;
		//framebufferInfo.pAttachments = attachments;
		//framebufferInfo.width = swapChainExtent.width;
		//framebufferInfo.height = swapChainExtent.height;
		//framebufferInfo.layers = 1;

		//if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
		//	throw std::runtime_error("failed to create framebuffer!");
		//}
	}

	VulkanFrameBuffer::~VulkanFrameBuffer()
	{

	}

}