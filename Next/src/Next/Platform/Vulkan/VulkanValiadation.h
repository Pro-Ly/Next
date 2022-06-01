#pragma once

namespace Next {

	class VulkanValiadation
	{
	public:
		static void CheckValidationLayerSupport();
		#ifdef NX_DEBUG
		static const bool enableValidationLayers = true;
		#else
		static const bool enableValidationLayers = false;
		#endif

		inline static  const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
	};
}


