#include "nxpch.h"
#include "VulkanRenderCommandBuffer.h"
#include "VulkanDevice.h"
#include "VulkanContext.h"

namespace Next {

	VulkanRenderCommandBuffer::VulkanRenderCommandBuffer()
	{
		auto device = VulkanContext::GetCurrentDevice()->GetLogicalDevice();
		auto physicalDevice = VulkanContext::GetCurrentDevice()->GetPhysicalDevice()->GetDevice();
		QueueFamilyIndices queueFamilyIndices = VulkanPhysicalDevice::findQueueFamilies(physicalDevice);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		if (vkCreateCommandPool(device, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS) {
			NX_CORE_ASSERT(false,"failed to create command pool!");
		}

		VkCommandBuffer commandBuffer;

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS) {
			NX_CORE_ASSERT(false,"failed to allocate command buffers!");
		}

		m_CommandBuffers.emplace_back(commandBuffer);//TODO

		Begin();
	}


	VulkanRenderCommandBuffer::~VulkanRenderCommandBuffer()
	{
		auto device = VulkanContext::GetCurrentDevice()->GetLogicalDevice();
		vkDestroyCommandPool(device, m_CommandPool, nullptr);
	}

	void VulkanRenderCommandBuffer::Begin()
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer(m_CommandBuffers[0], &beginInfo) != VK_SUCCESS) {
			NX_CORE_ASSERT(false,"failed to begin recording command buffer!");
		}
	}

}

