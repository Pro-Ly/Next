#pragma once

#include "Runtime/RenderSystem/IndexBuffer.h"
#include "VulkanAllocator.h"

namespace Next {

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(size_t bufferSize);
		virtual ~VulkanIndexBuffer() override;

		virtual void SetData(void* pData, size_t size, uint32_t bufferPosOffset = 0) override;
		virtual void Release() override;

		inline VkBuffer GetBuffer() { return m_Buffer; }
	private:
		VkBuffer m_Buffer;
		VkDeviceSize m_BufferSize;
		VmaAllocation m_Allocation;
	};
}


