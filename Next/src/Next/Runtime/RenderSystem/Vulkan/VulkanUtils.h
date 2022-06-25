#pragma once

#include <vulkan/vulkan.h>

#include <string>


namespace Next {

	#define NX_CHECK_VKRESULT(x)\
		{ if(x)\
			{ \
				NX_CORE_ERROR("VkResult is [{0}] in file [{1}] : line : [{2}]", ::Next::VulkanUtils::VkResultToString(x), __FILE__, __LINE__)\
				NX_CORE_ASSERT(false,"Vulkan Assert")\
			}\
		}



	// Macro to get a procedure address based on a vulkan instance
	#define GET_INSTANCE_PROC_ADDR(inst, entrypoint)                        \
	{                                                                       \
		fp##entrypoint = reinterpret_cast<PFN_vk##entrypoint>(vkGetInstanceProcAddr(inst, "vk"#entrypoint)); \
		NX_CORE_ASSERT(fp##entrypoint,"GET_INSTANCE_PROC_ADDR  WRONG");                                     \
	}

	// Macro to get a procedure address based on a vulkan device
	#define GET_DEVICE_PROC_ADDR(dev, entrypoint)                           \
	{                                                                       \
		fp##entrypoint = reinterpret_cast<PFN_vk##entrypoint>(vkGetDeviceProcAddr(dev, "vk"#entrypoint));   \
		NX_CORE_ASSERT(fp##entrypoint,"GET_DEVICE_PROC_ADDR  WRONG");                                     \
	}

	namespace VulkanUtils {

		const static std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		const static std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME,
			VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME,
		};

		#ifdef NX_DEBUG
		const static bool enableValidationLayers = true;
		#else
		const static bool enableValidationLayers = false;
		#endif

		const std::string&& VkResultToString(VkResult res);

	}
}


