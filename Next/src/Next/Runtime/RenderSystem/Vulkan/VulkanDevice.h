#pragma once

#include "VulkanUtils.h"
#include "Core/Ref.h"
#include "VulkanSwapChain.h"
#include "VulkanAllocator.h"

#include <vector>

namespace Next {

	class VulkanDevice final : public RefCounted
	{
	public:
		VulkanDevice(VkInstance vkInstance,VkSurfaceKHR surface);
		~VulkanDevice() = default;

		//Device
		inline const VkDevice GetVkDevice() const { return m_VkDevice; }
		inline const VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }

		//Queue
		inline const VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
		
		//Command Buffer
		inline VkCommandBuffer GetRenderCmdBuffer(int frameIdx) const { return m_RenderCmdBuffers[frameIdx]; }

		VkCommandBuffer BeginOneTimeCommand();
		void EndAndExecuteOneTimeCommand(VkCommandBuffer commandBuffer);

		void ResetAll(int frameIdx);

		//Image

		VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat findDepthFormat();

		inline VkSampleCountFlagBits GetSampleCountFlagBits() const { return m_MSAASamples; }
	private:
		struct QueueFamilyIndices {
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

			bool isComplete() {
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		};

		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;

		std::vector<VkCommandPool> m_RenderCmdPools;
		std::vector<VkCommandBuffer> m_RenderCmdBuffers;

		VkCommandPool m_DefaultCmdPool;

	private:
		VkInstance m_VkInstance;
		VkSurfaceKHR m_VkSurface;
		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_VkDevice;
		QueueFamilyIndices m_QueueFamilyIndices;

		//MSAA
		VkSampleCountFlagBits m_MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		VkImage m_ColorImage;
		VkImageView m_ColorImageView;
		VmaAllocation m_ColorResourceImageAllocation;

	private:
		void selectPhysicalDevice();
		void createVkDeviceAndQueue();
		Ref<VulkanSwapChain> createSwapChain();
		void createCommandPools();
		void createCommandBuffers();
		void createColorResources();
		QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
		VkSampleCountFlagBits getMaxUsableSampleCount();
	private:
		friend class VulkanContext;
	};
}


