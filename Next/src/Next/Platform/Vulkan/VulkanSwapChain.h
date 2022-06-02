#pragma once

#include "vulkan/vulkan.h"

#include "Next/Platform/Vulkan/VulkanContext.h"

namespace Next
{
	class VulkanSwapChain : public RefCounted
	{
	public:
		VulkanSwapChain();
		~VulkanSwapChain();

		void Init(VkInstance vkInstance, Ref<VulkanDevice>& device);
		void Create(GLFWwindow* window);

		void Destroy();
		static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice& device, VkSurfaceKHR surface);
	private:
		VkSwapchainKHR m_SwapChain;
		Ref<VulkanDevice> m_Device;
		std::vector<VkImage> m_SwapChainImages;
		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;
	private:
		
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities);
	};
}

