#pragma once

#include "VulkanUtils.h"
#include "Core/Ref.h"
#include "VulkanSwapChain.h"

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

		VkImageView CreateImageView(VkImage image, VkFormat format);

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

		Ref<VulkanSwapChain> m_VulkanSwapChain;

		std::vector<VkCommandPool> m_RenderCmdPools;
		std::vector<VkCommandBuffer> m_RenderCmdBuffers;

		VkCommandPool m_DefaultCmdPool;

	private:
		VkInstance m_VkInstance;
		VkSurfaceKHR m_VkSurface;
		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_VkDevice;
		QueueFamilyIndices m_QueueFamilyIndices;

	private:
		void selectPhysicalDevice();
		void createVkDeviceAndQueue();
		void createSwapChain();
		void createCommandPools();
		void createCommandBuffers();
		QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
	private:
		friend class VulkanContext;
	};
}


