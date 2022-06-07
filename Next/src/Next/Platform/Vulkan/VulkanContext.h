#pragma once

#include "Next/Renderer/RendererContext.h"
#include "Next/Platform/Vulkan/VulkanDevice.h"
#include "Next/Platform/Vulkan/VulkanValiadation.h"
#include "Next/Platform/Vulkan/VulkanSwapChain.h"
#include "Next/Renderer/RendererAPI.h"
#include "Next/Renderer/Renderer.h"
#include "VulkanImage.h"

namespace Next {

	class VulkanContext : public RendererContext
	{
	public:
		VulkanContext(GLFWwindow* windowHandle);
		virtual ~VulkanContext();
		virtual void Init() override;
		inline Ref<VulkanDevice> GetDevice() const { return m_vkDevice; }
		inline Ref<VulkanImage> GetImage() const { return m_Image; }

		inline static VkInstance  GetVKInstance() { return s_vkInstance; }
		inline static VkSurfaceKHR GetSurfaceKHR() { return s_SurfaceKHR; }

		inline static Ref<VulkanContext> Get() { return Ref<VulkanContext>(Renderer::GetContext()); }
		inline static Ref<VulkanDevice> GetCurrentDevice() { return Get()->GetDevice(); }

		void CreateImageViews(VulkanSwapChain& swapChain);
	private:
		GLFWwindow* m_Window;
		inline static VkInstance s_vkInstance = nullptr;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		Ref<VulkanPhysicalDevice> m_vkPhycicaldevice;
		Ref<VulkanDevice> m_vkDevice;
		Ref<VulkanImage> m_Image;
		VulkanSwapChain m_SwapChain;
		inline static VkSurfaceKHR s_SurfaceKHR;
	private:
		void InitVulkan();
		void CreateInstance();
		void SetupDebugMessenger();
		void CreateSurface();
		void PickPhysicalDevice();
		void CreateLogicalDevice();
		
		void CreateGraphicsPipeline();
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
	};
}

