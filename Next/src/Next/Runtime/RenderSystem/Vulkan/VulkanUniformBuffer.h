#pragma once

#include "VulkanUtils.h"
#include "VulkanAllocator.h"
#include "Runtime/RenderSystem/UniformBuffer.h"

namespace Next {
	class VulkanUniformBuffer : public UniformBuffer
	{
	public:
		VulkanUniformBuffer(size_t bufferSize);
		virtual ~VulkanUniformBuffer() override;

		virtual void Update() override;
		virtual void Release() override;

		inline VkBuffer GetBuffer() { return m_Buffer; }
	private:
		VkBuffer m_Buffer;
		VkDeviceSize m_BufferSize;
		VmaAllocation m_Allocation;
	};
}

