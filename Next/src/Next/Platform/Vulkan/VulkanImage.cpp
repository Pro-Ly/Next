#include "nxpch.h"
#include "VulkanImage.h"

namespace Next {


	VulkanImage::VulkanImage(VulkanSwapChain& swapChain, Ref<VulkanDevice> vkDevice)
	{
		auto swapChainImages = swapChain.GetSwapChainImages();

		m_VkDevice = vkDevice;
		
		m_SwapChainImageViews.resize(swapChainImages.size());
		for (size_t i = 0; i < swapChainImages.size(); i++) {
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = swapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = swapChain.GetSwapChainImageFormat();
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(vkDevice->GetLogicalDevice(), &createInfo, nullptr, &m_SwapChainImageViews[i]) != VK_SUCCESS) {
				NX_CORE_ASSERT(false,"failed to create image views!");
			}
		}
	}

	VulkanImage::~VulkanImage()
	{
		Release();
	}

	void VulkanImage::Release()
	{
		if (m_SwapChainImageViews.empty())
			return;
		for (auto imageView : m_SwapChainImageViews) {
			vkDestroyImageView(m_VkDevice->GetLogicalDevice(), imageView, nullptr);
		}
		m_SwapChainImageViews.clear();
	}

}