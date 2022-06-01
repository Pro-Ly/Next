#include "nxpch.h"
#include "VulkanDevice.h"
#include "Next/Platform/Vulkan/VulkanContext.h"


namespace Next {

	const std::vector<const char*> requireDeviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};


	VulkanPhysicalDevice::VulkanPhysicalDevice()
	{
		auto vkInstance = VulkanContext::GetVKInstance();
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);
		if (deviceCount == 0) {
			NX_CORE_ASSERT(false, "failed to find GPUs with Vulkan support!");
		}
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

		for (const auto& device : devices) {
			if (isDeviceSuitable(device)) {
				m_PhycicalDevice = device;
				break;
			}
		}

		if (m_PhycicalDevice == VK_NULL_HANDLE) {
			NX_CORE_ASSERT(false, "failed to find a suitable GPU!");
		}
	}


	VulkanPhysicalDevice::~VulkanPhysicalDevice()
	{

	}

	Ref<VulkanPhysicalDevice> VulkanPhysicalDevice::SelectOne()
	{
		return Ref<VulkanPhysicalDevice>::Create();
	}

	QueueFamilyIndices VulkanPhysicalDevice::findQueueFamilies(VkPhysicalDevice device)
	{
		auto vkInstance = VulkanContext::GetVKInstance();
		QueueFamilyIndices indices;
		// Logic to find queue family indices to populate struct with
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());


		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, VulkanContext::GetSurfaceKHR(), &presentSupport);

			if (presentSupport) {
				indices.presentFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}
			i++;
		}

		return indices;
	}

	bool VulkanPhysicalDevice::isDeviceSuitable(VkPhysicalDevice device)
	{
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		bool gpuSuitable = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
		bool hasFeatures = deviceFeatures.geometryShader;

		QueueFamilyIndices indices = findQueueFamilies(device);
		bool canProcessCommands = indices.isComplete();
		bool extensionsSupported = checkDeviceExtensionSupport(device);
		
		bool swapChainAdequate = false;
		if (extensionsSupported) {
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return gpuSuitable 
			&& hasFeatures 
			&& canProcessCommands 
			&& extensionsSupported
			&& swapChainAdequate;
	}

	bool VulkanPhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(requireDeviceExtensions.begin(), requireDeviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	SwapChainSupportDetails VulkanPhysicalDevice::querySwapChainSupport(VkPhysicalDevice device)
	{
		SwapChainSupportDetails details;
		auto surface = VulkanContext::GetSurfaceKHR();
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VulkanDevice::VulkanDevice(const Ref<VulkanPhysicalDevice>& physicalDevice)
		:m_PhycicalDevice(physicalDevice)
	{
		auto vkPhisicalDevice = m_PhycicalDevice->GetPhysicalDevice();
		QueueFamilyIndices indices = VulkanPhysicalDevice::findQueueFamilies(vkPhisicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(requireDeviceExtensions.size());
		createInfo.ppEnabledExtensionNames = requireDeviceExtensions.data();

		if (VulkanValiadation::enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanValiadation::validationLayers.size());
			createInfo.ppEnabledLayerNames = VulkanValiadation::validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(vkPhisicalDevice, &createInfo, nullptr, &m_LogicDevice) != VK_SUCCESS) {
			NX_CORE_ASSERT(false, "failed to create logical device!");
		}

		vkGetDeviceQueue(m_LogicDevice, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_LogicDevice, indices.presentFamily.value(), 0, &m_PresentQueue);
	}

	VulkanDevice::~VulkanDevice()
	{

	}

	void VulkanDevice::Destroy()
	{
		vkDestroyDevice(m_LogicDevice, nullptr);
	}



}