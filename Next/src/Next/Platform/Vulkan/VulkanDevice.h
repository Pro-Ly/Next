#pragma once

#include "Next/Core/Ref.h"
#include <vulkan/vulkan.h>

namespace Next
{
	class VulkanPhysicalDevice : public RefCounted
	{
	public:
		VulkanPhysicalDevice();
		~VulkanPhysicalDevice();

		static Ref<VulkanPhysicalDevice> SelectOne();
		inline VkPhysicalDevice GetPhysicalDevice() { return m_PhycicalDevice; }
	private:
		VkPhysicalDevice m_PhycicalDevice = nullptr;
	private:
		bool isDeviceSuitable(VkPhysicalDevice device);
	};

	class VulkanDevice : public RefCounted
	{
	public:
		VulkanDevice(const Ref<VulkanPhysicalDevice>& physicalDevice);
		~VulkanDevice();
		void Destroy();
	private:
		Ref<VulkanPhysicalDevice> m_PhycicalDevice = nullptr;
		VkDevice m_LogicDevice = nullptr;
		VkQueue m_GraphicsQueue = nullptr;
		VkQueue m_PresentQueue = nullptr;
	};
}

