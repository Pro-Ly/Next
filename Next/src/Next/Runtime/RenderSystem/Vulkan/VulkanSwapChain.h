#pragma once

#include "Core/Ref.h"
#include "VulkanUtils.h"

#include <vector>

namespace Next {

	class VulkanSwapChain : public RefCounted
	{
	public:
		struct InitInfo
		{
			uint32_t framesInFlight;
			VkInstance vkInstance;
			VkSurfaceKHR vkSurface;
			VkPhysicalDevice vkPhysicalDevice;
			VkDevice vkDevice;
			VkQueue presentQueue;
		};

		struct SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR        capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR>   presentModes;
		};

		VulkanSwapChain(InitInfo initInfo);
		~VulkanSwapChain();
		
		inline const VkSwapchainKHR Get() const { return m_VkSwapChain; }

		inline const std::vector<VkImageView> GetImageViews() const { return m_SwapChainImageViews; }
		inline const VkExtent2D GetExtend2D() const { return m_Extent; }
		inline const uint32_t GetCurImageIndex() const { return m_CurrentImageIndex; }
		inline const VkSurfaceFormatKHR GetSurfaceFormat() const { return m_SurfaceFormat; }
	
		void AcquireNextImage(VkSemaphore vkSemaphore);
		void Present(VkSemaphore signalSemaphores[]);
		void ReCreate();
	private:
		InitInfo m_InitInfo;

		VkSwapchainKHR m_VkSwapChain = VK_NULL_HANDLE;

		VkSurfaceFormatKHR m_SurfaceFormat;
		VkExtent2D m_Extent = {};

		std::vector<VkImageView> m_SwapChainImageViews;

		std::vector<VkImage> m_SwapChainImages;

		uint32_t m_CurrentImageIndex;

		const uint32_t DEFAULT_ACQUIRE_TIME_OUT = 10000000;
	private:
		SwapChainSupportDetails querySwapChainSupport();
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(std::vector<VkPresentModeKHR> availablePresentModes);
		VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR capabilities);

		void create();
		void cleanup();
	};
}


