#pragma once

#include "Next/Core/Ref.h"
#include <vulkan/vulkan.h>

namespace Next
{
	class VulkanPhycicalDevice : public RefCounted
	{
	public:
		VulkanPhycicalDevice();
		~VulkanPhycicalDevice();
		Ref<VulkanPhycicalDevice> Select();
	private:
		VkPhysicalDevice m_PhycicalDevice = nullptr;
	};
}

