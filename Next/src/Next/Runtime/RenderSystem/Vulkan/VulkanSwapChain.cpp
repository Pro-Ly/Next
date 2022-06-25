#include "nxpch.h"
#include "VulkanSwapChain.h"
#include "Core/Application.h"
#include "VulkanRenderer.h"

namespace Next {

	static PFN_vkGetPhysicalDeviceSurfaceSupportKHR fpGetPhysicalDeviceSurfaceSupportKHR;
	static PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
	static PFN_vkGetPhysicalDeviceSurfaceFormatsKHR fpGetPhysicalDeviceSurfaceFormatsKHR;
	static PFN_vkGetPhysicalDeviceSurfacePresentModesKHR fpGetPhysicalDeviceSurfacePresentModesKHR;
	static PFN_vkCreateSwapchainKHR fpCreateSwapchainKHR;
	static PFN_vkDestroySwapchainKHR fpDestroySwapchainKHR;
	static PFN_vkGetSwapchainImagesKHR fpGetSwapchainImagesKHR;
	static PFN_vkAcquireNextImageKHR fpAcquireNextImageKHR;
	static PFN_vkQueuePresentKHR fpQueuePresentKHR;
	// Nvidia extensions
	static PFN_vkCmdSetCheckpointNV fpCmdSetCheckpointNV;
	static PFN_vkGetQueueCheckpointDataNV fpGetQueueCheckpointDataNV;

	VulkanSwapChain::VulkanSwapChain(InitInfo initInfo)
		:m_InitInfo(initInfo)
	{
		// Create SwapChain Begin

		GET_DEVICE_PROC_ADDR(m_InitInfo.vkDevice, CreateSwapchainKHR);
		GET_DEVICE_PROC_ADDR(m_InitInfo.vkDevice, DestroySwapchainKHR);
		GET_DEVICE_PROC_ADDR(m_InitInfo.vkDevice, GetSwapchainImagesKHR);
		GET_DEVICE_PROC_ADDR(m_InitInfo.vkDevice, AcquireNextImageKHR);
		GET_DEVICE_PROC_ADDR(m_InitInfo.vkDevice, QueuePresentKHR);

		GET_INSTANCE_PROC_ADDR(m_InitInfo.vkInstance, GetPhysicalDeviceSurfaceCapabilitiesKHR);
		GET_INSTANCE_PROC_ADDR(m_InitInfo.vkInstance, GetPhysicalDeviceSurfaceFormatsKHR);
		GET_INSTANCE_PROC_ADDR(m_InitInfo.vkInstance, GetPhysicalDeviceSurfacePresentModesKHR);
		GET_INSTANCE_PROC_ADDR(m_InitInfo.vkInstance, CmdSetCheckpointNV);
		GET_INSTANCE_PROC_ADDR(m_InitInfo.vkInstance, GetQueueCheckpointDataNV);
		GET_INSTANCE_PROC_ADDR(m_InitInfo.vkInstance, GetPhysicalDeviceSurfaceSupportKHR);

		create();
	}

	VulkanSwapChain::~VulkanSwapChain()
	{

	}

	void VulkanSwapChain::AcquireNextImage(VkSemaphore vkSemaphore)
	{
		NX_CHECK_VKRESULT(vkAcquireNextImageKHR(
			m_InitInfo.vkDevice,
			m_VkSwapChain,
			DEFAULT_ACQUIRE_TIME_OUT,
			vkSemaphore,
			VK_NULL_HANDLE,
			&m_CurrentImageIndex));
	}

	void VulkanSwapChain::Present(VkSemaphore signalSemaphores[])
	{
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_VkSwapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &m_CurrentImageIndex;

		NX_CHECK_VKRESULT(vkQueuePresentKHR(m_InitInfo.presentQueue, &presentInfo));
	}

	void VulkanSwapChain::ReCreate()
	{
		cleanup();
		create();
	}

	VulkanSwapChain::SwapChainSupportDetails VulkanSwapChain::querySwapChainSupport()
	{
		SwapChainSupportDetails details_result;

		// capabilities
		NX_CHECK_VKRESULT(fpGetPhysicalDeviceSurfaceCapabilitiesKHR(m_InitInfo.vkPhysicalDevice, m_InitInfo.vkSurface, &details_result.capabilities));

		// formats
		uint32_t format_count;
		NX_CHECK_VKRESULT(fpGetPhysicalDeviceSurfaceFormatsKHR(m_InitInfo.vkPhysicalDevice, m_InitInfo.vkSurface, &format_count, nullptr));
		if (format_count != 0)
		{
			details_result.formats.resize(format_count);
			NX_CHECK_VKRESULT(fpGetPhysicalDeviceSurfaceFormatsKHR(
				m_InitInfo.vkPhysicalDevice, m_InitInfo.vkSurface, &format_count, details_result.formats.data()));
		}

		// present modes
		uint32_t presentmode_count;
		NX_CHECK_VKRESULT(fpGetPhysicalDeviceSurfacePresentModesKHR(m_InitInfo.vkPhysicalDevice, m_InitInfo.vkSurface, &presentmode_count, nullptr));
		if (presentmode_count != 0)
		{
			details_result.presentModes.resize(presentmode_count);
			NX_CHECK_VKRESULT(fpGetPhysicalDeviceSurfacePresentModesKHR(
				m_InitInfo.vkPhysicalDevice, m_InitInfo.vkSurface, &presentmode_count, details_result.presentModes.data()));
		}

		return details_result;
	}

	VkSurfaceFormatKHR VulkanSwapChain::chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> availableFormats)
	{
		//TODO : fit more cases
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
				availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR VulkanSwapChain::chooseSwapPresentMode(std::vector<VkPresentModeKHR> availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanSwapChain::chooseSwapExtent(VkSurfaceCapabilitiesKHR capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			uint32_t width = Application::Get().GetWindow().GetWidth();
			uint32_t height = Application::Get().GetWindow().GetHeight();

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = width;
			actualExtent.height = height;

			return actualExtent;
		}
	}



	void VulkanSwapChain::create()
	{
		SwapChainSupportDetails supportDetails = querySwapChainSupport();

		m_SurfaceFormat = chooseSwapSurfaceFormat(supportDetails.formats);

		VkPresentModeKHR presentMode = chooseSwapPresentMode(supportDetails.presentModes);

		m_Extent = chooseSwapExtent(supportDetails.capabilities);

		// Determine the number of images
		uint32_t desiredNumberOfSwapchainImages = supportDetails.capabilities.minImageCount + 1;
		if ((supportDetails.capabilities.maxImageCount > 0) &&
			(desiredNumberOfSwapchainImages > supportDetails.capabilities.maxImageCount))
		{
			desiredNumberOfSwapchainImages = supportDetails.capabilities.maxImageCount;
		}


		// Find the transformation of the surface
		VkSurfaceTransformFlagsKHR preTransform;
		if (supportDetails.capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
		{
			// We prefer a non-rotated transform
			preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}
		else
		{
			preTransform = supportDetails.capabilities.currentTransform;
		}

		// Find a supported composite alpha format (not all devices support alpha opaque)
		VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		// Simply select the first composite alpha format available
		std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
		};
		for (auto& compositeAlphaFlag : compositeAlphaFlags) {
			if (supportDetails.capabilities.supportedCompositeAlpha & compositeAlphaFlag) {
				compositeAlpha = compositeAlphaFlag;
				break;
			};
		}

		VkSwapchainCreateInfoKHR swapchainCI = {};
		swapchainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCI.pNext = NULL;
		swapchainCI.surface = m_InitInfo.vkSurface;
		swapchainCI.minImageCount = desiredNumberOfSwapchainImages;
		swapchainCI.imageFormat = m_SurfaceFormat.format;
		swapchainCI.imageColorSpace = m_SurfaceFormat.colorSpace;
		swapchainCI.imageExtent = { m_Extent.width, m_Extent.height };
		swapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainCI.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;
		swapchainCI.imageArrayLayers = 1;
		swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCI.queueFamilyIndexCount = 0;
		swapchainCI.pQueueFamilyIndices = NULL;
		swapchainCI.presentMode = presentMode;
		swapchainCI.oldSwapchain = m_VkSwapChain;
		// Setting clipped to VK_TRUE allows the implementation to discard rendering outside of the surface area
		swapchainCI.clipped = VK_TRUE;
		swapchainCI.compositeAlpha = compositeAlpha;

		// Enable transfer source on swap chain images if supported
		if (supportDetails.capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) {
			swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		}

		// Enable transfer destination on swap chain images if supported
		if (supportDetails.capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
			swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}

		NX_CHECK_VKRESULT(fpCreateSwapchainKHR(m_InitInfo.vkDevice, &swapchainCI, nullptr, &m_VkSwapChain));

		// Recreate Swap Chain Image Views
		//--------------------------

		NX_CHECK_VKRESULT(fpGetSwapchainImagesKHR(m_InitInfo.vkDevice, m_VkSwapChain, &desiredNumberOfSwapchainImages, NULL));

		m_SwapChainImageViews.resize(desiredNumberOfSwapchainImages);
		m_SwapChainImages.resize(desiredNumberOfSwapchainImages);

		NX_CHECK_VKRESULT(fpGetSwapchainImagesKHR(m_InitInfo.vkDevice, m_VkSwapChain, &desiredNumberOfSwapchainImages, m_SwapChainImages.data()));

		auto vulkanDevice = VulkanRenderer::GetContext()->m_VulkanDevice;

		for (uint32_t i = 0; i < desiredNumberOfSwapchainImages; i++)
		{
			m_SwapChainImageViews[i] = vulkanDevice->CreateImageView(m_SwapChainImages[i], m_SurfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT,1);
		}

	}

	void VulkanSwapChain::cleanup()
	{
		if (m_VkSwapChain != VK_NULL_HANDLE)
			fpDestroySwapchainKHR(m_InitInfo.vkDevice, m_VkSwapChain, nullptr);

		for (auto& imageView : m_SwapChainImageViews)
			vkDestroyImageView(m_InitInfo.vkDevice, imageView, nullptr);
		m_SwapChainImageViews.clear();
		m_SwapChainImages.clear();
	}

}