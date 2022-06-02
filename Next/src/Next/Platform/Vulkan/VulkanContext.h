#pragma once

#include "Next/Renderer/RendererContext.h"
#include "Vulkan/vulkan.h"
#include "Next/Platform/Vulkan/VulkanDevice.h"
#include "Next/Platform/Vulkan/VulkanValiadation.h"

namespace Next {

	class VulkanContext : public RendererContext
	{
	public:
		VulkanContext(GLFWwindow* windowHandle);
		virtual ~VulkanContext();
		virtual void Init() override;

		inline const static VkInstance  GetVKInstance() { return s_vkInstance; }
		inline const static VkSurfaceKHR GetSurfaceKHR() { return s_SurfaceKHR; }
		inline Ref<VulkanDevice> GetDevice() const { return m_vkDevice; }
	private:
		GLFWwindow* m_Window;
		inline static VkInstance s_vkInstance = nullptr;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		Ref<VulkanPhysicalDevice> m_vkPhycicaldevice;
		Ref<VulkanDevice> m_vkDevice;
		inline static VkSurfaceKHR s_SurfaceKHR;
	private:
		void InitVulkan();
		void CreateInstance();
		void SetupDebugMessenger();
		void CreateSurface();
		void PickPhysicalDevice();
		void CreateLogicalDevice();
		void CreateSwapChain();
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
	};
}

