#pragma once

#include "Core/Config.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderPass.h"
#include "VulkanFrameBuffer.h"
#include "VulkanPipeline.h"
#include "VulkanAllocator.h"
#include "VulkanIndexBuffer.h"
#include "VulkanUniformBuffer.h"
#include "VulkanImage.h"

#include "Runtime/RenderSystem/RenderSystem.h"


#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <optional>
#include <set>

namespace Next {

	class VulkanContext final : public RefCounted
	{
	public:
		VulkanContext();
		~VulkanContext();

		void Init();
		void ShutDown();
		void BeginFrame();
		void DrawFrame();
		void EndFrame();

	public:
		const RendererConfig& m_Config = RenderSystem::GetConfig();
		VkInstance m_VkInstance;
		VkDebugUtilsMessengerEXT debugMessenger;
		Ref<VulkanDevice> m_VulkanDevice;
		VkQueue m_GraphicsQueue;
		//VkFormat depthFormat;
		VkSurfaceKHR m_VkSurface;
		Ref<VulkanSwapChain> m_VulkanSwapChain;
		std::vector<VkSemaphore> presentFinishedSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> allCompleteFences;
		Ref<VulkanRenderPass> m_MainRenderPass;
		std::vector<Ref<VulkanFrameBuffer>> m_SwapChainFramebuffers;
		Ref<VulkanPipeline> m_VulkanPipeline;
		uint32_t m_CurFrameIndex = 0;
		uint32_t m_CurSwapChainImageIndex;
		Ref<VulkanVertexBuffer> m_VulkanVertexBuffer;
		Ref<VulkanIndexBuffer> m_VulkanIndexBuffer;
		Ref<VulkanImage> m_VulkanImage;
		std::vector<Ref<VulkanUniformBuffer>> m_VulkanUniformBuffer;

		VkDescriptorSetLayout descriptorSetLayout;
		std::vector<VkDescriptorSet> descriptorSets;

		VkDescriptorPool m_UniformDescriptorPool;

	private:
		void createVkInstance();
		void setupDebugMessenger();
		bool checkValidationLayerSupport();
		std::vector<const char*> getRequiredExtensions();
		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	private:
		friend class VulkanRenderer;
	};





	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

	VKAPI_ATTR VkBool32 VKAPI_CALL VkDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
}


