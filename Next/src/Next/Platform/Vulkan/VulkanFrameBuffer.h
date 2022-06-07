#pragma once

#include "Next/Renderer/FrameBuffer.h"
#include <vulkan/vulkan.h>
namespace Next {

	class VulkanFrameBuffer : public FrameBuffer
	{
	public:
		VulkanFrameBuffer();
		virtual ~VulkanFrameBuffer() override;

	private:
		//VkRenderPass m_RenderPass;
	};
}


