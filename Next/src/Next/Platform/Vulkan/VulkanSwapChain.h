#pragma once

#include "Next/Platform/Vulkan/VulkanDevice.h"

struct GLFWwindow;

namespace Next
{
	class VulkanSwapChain : public RefCounted
	{
	public:
		VulkanSwapChain();
		~VulkanSwapChain();

		void Init(VkInstance vkInstance, Ref<VulkanDevice>& device);
		void Create(GLFWwindow* window);

		inline const std::vector<VkImage> GetSwapChainImages() const { return m_SwapChainImages; }
		inline const VkFormat GetSwapChainImageFormat() const { return m_SwapChainImageFormat; }
		inline const VkExtent2D GetExtend2D() const { return m_SwapChainExtent; }
		inline const VkRenderPass GetRenderPass() const { return m_RenderPass; }
		void Destroy();
		static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice& device, VkSurfaceKHR surface);
	private:
		VkRenderPass m_RenderPass;
		VkSwapchainKHR m_SwapChain = nullptr;
		Ref<VulkanDevice> m_Device = nullptr;
		std::vector<VkImage> m_SwapChainImages;
		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;
	private:

		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities);
	};
}

