#pragma once

#include "VulkanDevice.h"
#include "VulkanMemoryAllocator/vk_mem_alloc.h"

#include <string>

namespace Next {

	class VulkanAllocator final
	{
		struct VulkanAllocatorData
		{
			VmaAllocator Allocator;
			uint64_t TotalAllocatedBytes = 0;
		};

	public:
		VulkanAllocator() : m_Tag("Default") {}
		VulkanAllocator(const std::string& tag);
		~VulkanAllocator();
		static void Init(Ref<VulkanDevice> device, VkInstance instance);
		static void Shutdown();

		static VmaAllocator& GetVMAAllocator();

		template<typename T>
		T* MapMemory(VmaAllocation allocation)
		{
			T* mappedMemory;
			vmaMapMemory(VulkanAllocator::GetVMAAllocator(), allocation, (void**)&mappedMemory);
			return mappedMemory;
		}

		void UnmapMemory(VmaAllocation allocation);
		
		VmaAllocation AllocateBuffer(VkBufferCreateInfo bufferCreateInfo, VkMemoryPropertyFlags flag, VkBuffer& outBuffer);
		void DestroyBuffer(VkBuffer buffer, VmaAllocation allocation);

		VmaAllocation AllocateImage(VkImageCreateInfo imageCreateInfo, VkMemoryPropertyFlags flags, VkImage& outImage);
		void DestroyImage(VkImage image, VmaAllocation allocation);

	private:
		const std::string& m_Tag;
		static VulkanAllocatorData* s_Data;
	};

}

