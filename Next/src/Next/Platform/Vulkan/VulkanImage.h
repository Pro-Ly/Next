#pragma once

#include "Next/Core/Ref.h"
#include "Next/Platform/Vulkan/VulkanSwapChain.h"
#include "Next/Platform/Vulkan/VulkanDevice.h"

namespace Next {

	class VulkanImage : public RefCounted
	{
	public:
		VulkanImage(VulkanSwapChain& swapChain, Ref<VulkanDevice> vkDevice);
		~VulkanImage();
	private:
		std::vector<VkImageView> m_SwapChainImageViews;
		Ref<VulkanDevice> m_VkDevice;
	};
}


