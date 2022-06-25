#include "nxpch.h"
#include "VulkanDevice.h"
#include "Runtime/RenderSystem/RenderSystem.h"
#include "VulkanAllocator.h"
#include "VulkanRenderer.h"

#include <vector>
#include <set>

namespace Next {

	VulkanDevice::VulkanDevice(VkInstance vkInstance, VkSurfaceKHR surface)
		:m_VkInstance(vkInstance), m_VkSurface(surface)
	{
		selectPhysicalDevice();
		createVkDeviceAndQueue();
		createCommandPools();
		createCommandBuffers();
	}

	VkCommandBuffer VulkanDevice::BeginOneTimeCommand()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_DefaultCmdPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(m_VkDevice, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void VulkanDevice::EndAndExecuteOneTimeCommand(VkCommandBuffer commandBuffer)
	{
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(m_GraphicsQueue);

		vkFreeCommandBuffers(m_VkDevice, m_DefaultCmdPool, 1, &commandBuffer);

	}

	void VulkanDevice::ResetAll(int frameIdx)
	{
		NX_CHECK_VKRESULT(vkResetCommandPool(m_VkDevice, m_RenderCmdPools[frameIdx], 0));
	}


	void VulkanDevice::selectPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_VkInstance, &deviceCount, nullptr);

		NX_CORE_ASSERT(deviceCount, "failed to find GPUs with Vulkan support!");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_VkInstance, &deviceCount, devices.data());

		for (const auto& device : devices) {
			VkPhysicalDeviceProperties deviceProperty;
			vkGetPhysicalDeviceProperties(device, &deviceProperty);
			//only use Discrete GPU ( ∂¿¡¢œ‘ø® ) £°
			if (deviceProperty.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				m_PhysicalDevice = device;
				//Scan Supported Features
				VkPhysicalDeviceFeatures supportedFeatures;
				vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
				
				m_MSAASamples = getMaxUsableSampleCount();
				break;
			}
		}

		NX_CORE_ASSERT(m_PhysicalDevice, "failed to find a suitable GPU!");
	}

	void VulkanDevice::createVkDeviceAndQueue()
	{
		// Get Available Extensions Begin

		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extensionCount, availableExtensions.data());

		// Get Available Extensions End

		// Create Logical Device Begin

		m_QueueFamilyIndices = getQueueFamilies(m_PhysicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = {
			m_QueueFamilyIndices.graphicsFamily.value(),
			m_QueueFamilyIndices.presentFamily.value() };

		float defaultQueuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &defaultQueuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}


		// Since all depth formats may be optional, we need to find a suitable depth format to use
		// Start with the highest precision packed format
		std::vector<VkFormat> depthFormats = {
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM
		};


		//for (auto& format : depthFormats)
		//{
		//	VkFormatProperties formatProps;
		//	vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice, format, &formatProps);
		//	// Format must support depth stencil attachment for optimal tiling
		//	if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
		//	{
		//		depthFormat = format;
		//		break;
		//	}
		//}

		// Enabled Features
		VkPhysicalDeviceFeatures enabledFeatures{};
		enabledFeatures.samplerAnisotropy = true;
		enabledFeatures.wideLines = true;
		enabledFeatures.fillModeNonSolid = true;
		enabledFeatures.independentBlend = true;
		enabledFeatures.pipelineStatisticsQuery = true;
		enabledFeatures.sampleRateShading = VK_TRUE;

		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
		deviceCreateInfo.pEnabledFeatures = &enabledFeatures;
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(VulkanUtils::deviceExtensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = VulkanUtils::deviceExtensions.data();

		if (VulkanUtils::enableValidationLayers) {
			deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(VulkanUtils::validationLayers.size());
			deviceCreateInfo.ppEnabledLayerNames = VulkanUtils::validationLayers.data();
		}
		else {
			deviceCreateInfo.enabledLayerCount = 0;
		}

		NX_CHECK_VKRESULT(vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, nullptr, &m_VkDevice));

		vkGetDeviceQueue(m_VkDevice, m_QueueFamilyIndices.graphicsFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_VkDevice, m_QueueFamilyIndices.presentFamily.value(), 0, &m_PresentQueue);

		// Create Logical Device End
	}

	Ref<VulkanSwapChain> VulkanDevice::createSwapChain()
	{
		const RendererConfig& rendererConfig = RenderSystem::GetConfig();
		VulkanSwapChain::InitInfo swapChainInitInfo = {};
		swapChainInitInfo.framesInFlight = rendererConfig.FramesInFlight;
		swapChainInitInfo.vkInstance = m_VkInstance;
		swapChainInitInfo.vkSurface = m_VkSurface;
		swapChainInitInfo.vkPhysicalDevice = m_PhysicalDevice;
		swapChainInitInfo.vkDevice = m_VkDevice;
		swapChainInitInfo.presentQueue = m_PresentQueue;
		return Ref<VulkanSwapChain>::Create(swapChainInitInfo);
	}

	void VulkanDevice::createCommandPools()
	{
		const RendererConfig& config = RenderSystem::GetConfig();

		//Create Render CommandPools
		m_RenderCmdPools.resize(config.FramesInFlight);

		VkCommandPoolCreateInfo renderPoolInfo{};
		renderPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		renderPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		renderPoolInfo.queueFamilyIndex = m_QueueFamilyIndices.graphicsFamily.value();

		for (int i = 0; i < m_RenderCmdPools.size(); i++)
		{
			NX_CHECK_VKRESULT(vkCreateCommandPool(m_VkDevice, &renderPoolInfo, nullptr, &m_RenderCmdPools[i]));
		}

		//Create Default Command Pool
		VkCommandPoolCreateInfo defaultPoolInfo{};
		defaultPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		defaultPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		defaultPoolInfo.queueFamilyIndex = m_QueueFamilyIndices.graphicsFamily.value();

		NX_CHECK_VKRESULT(vkCreateCommandPool(m_VkDevice, &defaultPoolInfo, nullptr, &m_DefaultCmdPool));
	}

	void VulkanDevice::createCommandBuffers()
	{
		const RendererConfig& config = RenderSystem::GetConfig();

		//Create Render CommandBuffers
		m_RenderCmdBuffers.resize(config.FramesInFlight);

		for (int i = 0; i < m_RenderCmdPools.size(); i++)
		{
			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = m_RenderCmdPools[i];
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = 1;

			NX_CHECK_VKRESULT(vkAllocateCommandBuffers(m_VkDevice,&allocInfo,&m_RenderCmdBuffers[i]));
		}
	}

	void VulkanDevice::createColorResources()
	{
		auto swapChain = VulkanRenderer::GetContext()->m_VulkanSwapChain;
		VkFormat colorFormat = swapChain->GetSurfaceFormat().format;
		VkExtent2D extent = swapChain->GetExtend2D();

		VulkanAllocator allocator("Color Resource");

		//Create VkImage
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = static_cast<uint32_t>(extent.width);
		imageInfo.extent.height = static_cast<uint32_t>(extent.height);
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1; // optional
		imageInfo.arrayLayers = 1;
		imageInfo.format = colorFormat;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = m_MSAASamples;
		imageInfo.flags = 0; // Optional

		m_ColorResourceImageAllocation = allocator.AllocateImage(imageInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_ColorImage);

		m_ColorImageView = CreateImageView(m_ColorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}

	VulkanDevice::QueueFamilyIndices VulkanDevice::getQueueFamilies(VkPhysicalDevice device) {
		QueueFamilyIndices indices;

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
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_VkSurface, &presentSupport);

			if (presentSupport) {
				indices.presentFamily = i;
			}

			if (indices.isComplete())
			{
				break;
			}
			i++;
		}

		return indices;
	}

	VkImageView VulkanDevice::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) {
		//Create Image View Begin

		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;

		NX_CHECK_VKRESULT(vkCreateImageView(m_VkDevice, &viewInfo, nullptr, &imageView));

		return imageView;
	}

	VkFormat VulkanDevice::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice, format, &props);
			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}

		NX_CORE_ASSERT(false, "failed to find supported format!");

	}

	VkFormat VulkanDevice::findDepthFormat()
	{
		return findSupportedFormat(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	VkSampleCountFlagBits VulkanDevice::getMaxUsableSampleCount()
	{
		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &physicalDeviceProperties);

		VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
		if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
		if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
		if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
		if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
		if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
		if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

		return VK_SAMPLE_COUNT_1_BIT;
	}

}
