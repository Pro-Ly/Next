#pragma once

#include "Next/Renderer/RenderCommandBuffer.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace Next {

	class VulkanRenderCommandBuffer : public RenderCommandBuffer
	{
	public:
		VulkanRenderCommandBuffer();
		virtual ~VulkanRenderCommandBuffer() override;

		inline VkCommandBuffer GetCommandBuffer(int index) const { return m_CommandBuffers[index]; }

		void Begin();
	private:
		VkCommandPool m_CommandPool;
		std::vector<VkCommandBuffer> m_CommandBuffers;
	};
}


