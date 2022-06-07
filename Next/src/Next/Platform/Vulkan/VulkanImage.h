#pragma once

#include "Next/Core/Ref.h"
#include "Next/Platform/Vulkan/VulkanSwapChain.h"
#include "Next/Platform/Vulkan/VulkanDevice.h"

#include <vector>

namespace Next {

	class VulkanImage : public RefCounted
	{
	public:
		VulkanImage(VulkanSwapChain& swapChain, Ref<VulkanDevice> vkDevice);
		~VulkanImage();
		inline std::vector<VkImageView> GetSwapChainImageViews() const { return m_SwapChainImageViews; }
		void Release();
	private:
		std::vector<VkImageView> m_SwapChainImageViews;
		Ref<VulkanDevice> m_VkDevice;
	};
}


