#pragma once

#include "Runtime/RenderSystem/VertexBuffer.h"
#include "VulkanUtils.h"
#include "VulkanAllocator.h"

#include <vector>

namespace Next {

	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(size_t bufferSize);
		virtual ~VulkanVertexBuffer() override;

		virtual void SetData(void* pData, size_t size, uint32_t bufferPosOffset = 0);
		virtual void Release();

		inline VkBuffer GetBuffer() { return m_Buffer; }
	private:
		VkBuffer m_Buffer;
		VkDeviceSize m_BufferSize;
		VmaAllocation m_Allocation;
	};

	static VkFormat ShaderDataTypeToVulkanFormat(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:     return VK_FORMAT_R32_SFLOAT;
			case ShaderDataType::Float2:    return VK_FORMAT_R32G32_SFLOAT;
			case ShaderDataType::Float3:    return VK_FORMAT_R32G32B32_SFLOAT;
			case ShaderDataType::Float4:    return VK_FORMAT_R32G32B32A32_SFLOAT;
		}
		NX_CORE_ASSERT(false, "Unknow Type!");
		return VK_FORMAT_UNDEFINED;
	}

}

