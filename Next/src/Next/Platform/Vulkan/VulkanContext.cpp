#include "nxpch.h"
#include "VulkanContext.h"

#include "Next\Core.h"

#include <GLFW/glfw3.h>

namespace Next {

	VulkanContext::VulkanContext(GLFWwindow* windowHandle)
		:m_Window(windowHandle)
	{

	}

	VulkanContext::~VulkanContext()
	{
		if (VulkanValiadation::enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(s_vkInstance, m_DebugMessenger, nullptr);
		}
		m_vkDevice->Destroy();
		vkDestroySurfaceKHR(s_vkInstance, s_SurfaceKHR, nullptr);
		vkDestroyInstance(s_vkInstance, nullptr);
		s_vkInstance = nullptr;
	}

	void VulkanContext::Init()
	{
		InitVulkan();
	}

	void VulkanContext::InitVulkan()
	{
		VulkanValiadation::CheckValidationLayerSupport();
		CreateInstance();
		SetupDebugMessenger();
		CreateSurface();
		PickPhysicalDevice();
		CreateLogicalDevice();
	}

	void VulkanContext::CreateInstance()
	{
		#pragma region Create Instance
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		//Get Required Extensions
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		if (VulkanValiadation::enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (VulkanValiadation::enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanValiadation::validationLayers.size());
			createInfo.ppEnabledLayerNames = VulkanValiadation::validationLayers.data();

			debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debugCreateInfo.pfnUserCallback = debugCallback;
			debugCreateInfo.pUserData = nullptr; // Optional

			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else {
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		VkResult result = vkCreateInstance(&createInfo, nullptr, &s_vkInstance);
		NX_CORE_ASSERT(!result, "Create VkInstance Failed!");

		#pragma endregion
	}

	void VulkanContext::SetupDebugMessenger()
	{
		if (!VulkanValiadation::enableValidationLayers) return;

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

		//Populate DebugMessenger CreateInfo
		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugCreateInfo.pfnUserCallback = debugCallback;
		debugCreateInfo.pUserData = nullptr; // Optional

		if (CreateDebugUtilsMessengerEXT(s_vkInstance, &debugCreateInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS) {
			NX_CORE_ASSERT(false, ("failed to set up debug messenger!"));
		}
	}

	void VulkanContext::CreateSurface()
	{
		if (glfwCreateWindowSurface(s_vkInstance, m_Window, nullptr, &s_SurfaceKHR) != VK_SUCCESS) {
			NX_CORE_ASSERT(false,"failed to create window surface!");
		}
	}

	void VulkanContext::PickPhysicalDevice()
	{
		m_vkPhycicaldevice = VulkanPhysicalDevice::SelectOne();
	}

	void VulkanContext::CreateLogicalDevice()
	{
		m_vkDevice = Ref<VulkanDevice>::Create(m_vkPhycicaldevice);
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanContext::debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
			// Message is important enough to show
			NX_CORE_WARN("validation layer:{0} ", pCallbackData->pMessage);
		}

		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
			// Message is important enough to show
			NX_CORE_ERROR("validation layer:{0} ", pCallbackData->pMessage);
		}

		return VK_FALSE;
	}

	VkResult VulkanContext::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void VulkanContext::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

}