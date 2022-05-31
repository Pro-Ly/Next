#pragma once

#include "Next/Renderer/RendererContext.h"
#include "Vulkan/vulkan.h"

namespace Next {

	class VulkanContext : public RendererContext
	{
	public:
		VulkanContext(GLFWwindow* windowHandle);
		virtual ~VulkanContext();
		virtual void Create() override;
		inline static VkInstance  GetVKInstance() { return s_vkInstance; }
	private:
		GLFWwindow* m_Window;
		inline static VkInstance s_vkInstance = nullptr;
		VkDebugUtilsMessengerEXT m_DebugMessenger;

	private:
		void InitVulkan();
		void SetupDebugMessenger();
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
	};
}

