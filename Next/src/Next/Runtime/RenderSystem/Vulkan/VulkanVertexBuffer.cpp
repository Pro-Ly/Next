#include "nxpch.h"
#include "VulkanVertexBuffer.h"
#include "VulkanRenderer.h"

namespace Next {

	VulkanVertexBuffer::VulkanVertexBuffer(size_t bufferSize)
		:m_BufferSize(bufferSize)
	{
		VulkanAllocator allocator("VertexBuffer");

		//Create Vertex Buffer

		VkBufferCreateInfo vertexBufferCreateInfo{};
		vertexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		vertexBufferCreateInfo.size = m_BufferSize;
		vertexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		vertexBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		m_Allocation = allocator.AllocateBuffer(vertexBufferCreateInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Buffer);

	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		Release();
	}


	void VulkanVertexBuffer::SetData(void* pData, size_t size, uint32_t bufferPosOffset /*= 0*/)
	{
		Ref<VulkanDevice> vulkanDevice = VulkanRenderer::GetContext()->m_VulkanDevice;

		VulkanAllocator allocator("VertexBuffer");

		//Create Staging Buffer
		VkBuffer stagingBuffer;
		VkBufferCreateInfo stagingBufferCreateInfo{};
		stagingBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferCreateInfo.size = size;
		stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		stagingBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		auto stagingBufferAllocation = allocator.AllocateBuffer(stagingBufferCreateInfo,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer);

		//Write into Staing Buffer
		uint8_t* destData = allocator.MapMemory<uint8_t>(stagingBufferAllocation);
		memcpy(destData, pData, size);
		allocator.UnmapMemory(stagingBufferAllocation);

		VkCommandBuffer commandBuffer = vulkanDevice->BeginOneTimeCommand();

		// transfer
		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		copyRegion.dstOffset = bufferPosOffset;
		vkCmdCopyBuffer(commandBuffer, stagingBuffer, m_Buffer, 1, &copyRegion);

		vulkanDevice->EndAndExecuteOneTimeCommand(commandBuffer);

		allocator.DestroyBuffer(stagingBuffer, stagingBufferAllocation);
	}

	void VulkanVertexBuffer::Release()
	{
		if (m_Allocation)
		{
			VulkanAllocator allocator("VertexBuffer");
			allocator.DestroyBuffer(m_Buffer, m_Allocation);
			m_Allocation = nullptr;
		}
	}

}