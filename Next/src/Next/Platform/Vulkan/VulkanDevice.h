#pragma once

#include "Next/Core/Ref.h"
#include "vulkan/vulkan.h"
#include <optional>
#include <set>

namespace Next
{
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VulkanPhysicalDevice : public RefCounted
	{
	public:
		VulkanPhysicalDevice();
		~VulkanPhysicalDevice();

		static Ref<VulkanPhysicalDevice> SelectOne();
		const inline VkPhysicalDevice GetDevice() const { return m_PhycicalDevice; }

		static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	private:
		VkPhysicalDevice m_PhycicalDevice = nullptr;
	private:
		bool isDeviceSuitable(VkPhysicalDevice device);

		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	};

	class VulkanDevice : public RefCounted
	{
	public:
		VulkanDevice(const Ref<VulkanPhysicalDevice>& physicalDevice);
		~VulkanDevice();
		void Destroy();
		Ref<VulkanPhysicalDevice> GetPhysicalDevice() const { return m_PhycicalDevice; }
		const VkDevice GetLogicalDevice() const { return m_LogicDevice; }
	private:
		Ref<VulkanPhysicalDevice> m_PhycicalDevice = nullptr;
		VkDevice m_LogicDevice = nullptr;
		VkQueue m_GraphicsQueue = nullptr;
		VkQueue m_PresentQueue = nullptr;
	private:
		
	};
}

