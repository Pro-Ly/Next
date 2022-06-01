#include "nxpch.h"
#include "VulkanValiadation.h"
#include "Next/Core.h"

#include "vulkan/vulkan.h"

namespace Next {

	void VulkanValiadation::CheckValidationLayerSupport()
	{
		if (!VulkanValiadation::enableValidationLayers)
			return;

		bool checkValidationLayerSupport = false;

		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());


		for (const char* layerName : validationLayers) {
			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					checkValidationLayerSupport = true;
					break;
				}
			}
		}
		if (!checkValidationLayerSupport) {
			NX_CORE_ASSERT(false, "validation layers requested, but not available!");
		}
	}

}