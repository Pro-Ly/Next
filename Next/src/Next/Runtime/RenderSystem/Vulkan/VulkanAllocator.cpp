#include "nxpch.h"
#include "VulkanAllocator.h"
#include "VulkanRenderer.h"

namespace Next {

	VulkanAllocator::VulkanAllocatorData* VulkanAllocator::s_Data = nullptr;

	VulkanAllocator::VulkanAllocator(const std::string& tag)
		:m_Tag(tag)
	{

	}

	VulkanAllocator::~VulkanAllocator()
	{

	}

	void VulkanAllocator::Init(VkInstance instance)
	{
		auto device = VulkanRenderer::GetContext()->m_VulkanDevice;

		s_Data = new VulkanAllocatorData();

		VmaAllocatorCreateInfo allocatorInfo = {};
		allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_2;
		allocatorInfo.physicalDevice = device->GetPhysicalDevice();
		allocatorInfo.device = device->GetVkDevice();
		allocatorInfo.instance = instance;

		vmaCreateAllocator(&allocatorInfo, &s_Data->Allocator);
	}

	VmaAllocation VulkanAllocator::AllocateBuffer(VkBufferCreateInfo bufferCreateInfo, VkMemoryPropertyFlags flag, VkBuffer& outBuffer)
	{
		VmaAllocationCreateInfo allocCreateInfo = {};
		allocCreateInfo.requiredFlags = flag;

		VmaAllocation allocation;
		vmaCreateBuffer(s_Data->Allocator, &bufferCreateInfo, &allocCreateInfo, &outBuffer, &allocation, nullptr);

		// TODO: Tracking
		VmaAllocationInfo allocInfo{};
		vmaGetAllocationInfo(s_Data->Allocator, allocation, &allocInfo);
		NX_CORE_INFO("VulkanAllocator ({0}): allocating buffer; size = {1} Bytes", m_Tag, allocInfo.size);

		s_Data->TotalAllocatedBytes += allocInfo.size;
		NX_CORE_INFO("VulkanAllocator ({0}): total allocated since start is {1} Bytes", m_Tag, s_Data->TotalAllocatedBytes);



		return allocation;
	}

	VmaAllocation VulkanAllocator::AllocateImage(VkImageCreateInfo imageCreateInfo, VkMemoryPropertyFlags flags, VkImage& outImage)
	{
		VmaAllocationCreateInfo allocCreateInfo = {};
		allocCreateInfo.requiredFlags = flags;

		VmaAllocation allocation;
		vmaCreateImage(s_Data->Allocator, &imageCreateInfo, &allocCreateInfo, &outImage, &allocation, nullptr);

		// TODO: Tracking
		VmaAllocationInfo allocInfo;
		vmaGetAllocationInfo(s_Data->Allocator, allocation, &allocInfo);
		NX_CORE_INFO("VulkanAllocator ({0}): allocating image; size = {1} Bytes", m_Tag, allocInfo.size);

		s_Data->TotalAllocatedBytes += allocInfo.size;
		NX_CORE_INFO("VulkanAllocator ({0}): total allocated since start is {1} Bytes", m_Tag, s_Data->TotalAllocatedBytes);


		return allocation;
	}

	void VulkanAllocator::DestroyImage(VkImage image, VmaAllocation allocation)
	{
		NX_CORE_ASSERT(image && allocation, "");
		vmaDestroyImage(s_Data->Allocator, image, allocation);
	}

	void VulkanAllocator::UnmapMemory(VmaAllocation allocation)
	{
		vmaUnmapMemory(s_Data->Allocator, allocation);
	}

	void VulkanAllocator::Shutdown()
	{
		vmaDestroyAllocator(s_Data->Allocator);
		delete(s_Data);
		s_Data = nullptr;
	}

	VmaAllocator& VulkanAllocator::GetVMAAllocator()
	{
		return s_Data->Allocator;
	}

	void VulkanAllocator::DestroyBuffer(VkBuffer buffer, VmaAllocation allocation)
	{
		NX_CORE_ASSERT(buffer && allocation, "");
		vmaDestroyBuffer(s_Data->Allocator, buffer, allocation);
	}
}