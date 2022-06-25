#include "nxpch.h"
#include "VulkanContext.h"
#include "Core/Application.h"
#include "VulkanRenderer.h"

#include <filesystem>

namespace Next {

	const std::vector<VertexBuffer::Vertex> vertices = {
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},

		{{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
		{{0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
		{{0.5f, 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
		{{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}


	};

	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0,
		4, 5 ,6 ,6 ,7 ,4
	};

	VulkanContext::VulkanContext()
	{

	}

	VulkanContext::~VulkanContext()
	{
		//VulkanAllocator::Shutdown();


	}

	void VulkanContext::Init()
	{
		createVkInstance();

		setupDebugMessenger();

		NX_CHECK_VKRESULT(glfwCreateWindowSurface(m_VkInstance, (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow(), nullptr, &m_VkSurface));

		m_VulkanDevice = Ref<VulkanDevice>::Create(m_VkInstance, m_VkSurface);

		m_VulkanDevice->createSwapChain();

		m_GraphicsQueue = m_VulkanDevice->GetGraphicsQueue();

		m_VulkanSwapChain = m_VulkanDevice->m_VulkanSwapChain;

		VulkanAllocator::Init(m_VulkanDevice, m_VkInstance);

		auto device = m_VulkanDevice->GetVkDevice();

		// Create Synchronization Objects Begin

		presentFinishedSemaphores.resize(m_Config.FramesInFlight);
		renderFinishedSemaphores.resize(m_Config.FramesInFlight);
		allCompleteFences.resize(m_Config.FramesInFlight);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < m_Config.FramesInFlight; i++) {
			NX_CHECK_VKRESULT(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &presentFinishedSemaphores[i]));
			NX_CHECK_VKRESULT(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]));
			NX_CHECK_VKRESULT(vkCreateFence(device, &fenceInfo, nullptr, &allCompleteFences[i]));
		}

		// Create Synchronization Objects End

		m_MainRenderPass = Ref<VulkanRenderPass>::Create(device, m_VulkanSwapChain->GetSurfaceFormat());

		// Create FrameBuffers for every swapchain image Begin

		m_SwapChainFramebuffers.resize(m_VulkanSwapChain->GetImageViews().size());
		for (uint32_t i = 0; i < m_SwapChainFramebuffers.size(); i++)
		{
			VulkanFrameBuffer::InitInfo frameBufferInitInfo = {};
			frameBufferInitInfo.vkDevice = m_VulkanDevice->GetVkDevice();
			frameBufferInitInfo.vkRenderPass = m_MainRenderPass->GetPass();
			frameBufferInitInfo.vkImageView = m_VulkanSwapChain->GetImageViews()[i];
			frameBufferInitInfo.width = m_VulkanSwapChain->GetExtend2D().width;
			frameBufferInitInfo.height = m_VulkanSwapChain->GetExtend2D().height;
			m_SwapChainFramebuffers[i] = Ref<VulkanFrameBuffer>::Create(frameBufferInitInfo);
		}

		// Create FrameBuffers for every swapchain image End


		m_VulkanImage = Ref<VulkanImage>::Create("assets/textures/texture.jpg");


		//Pipeline Begin
		m_VulkanPipeline = Ref<VulkanPipeline>::Create(m_VulkanDevice->GetVkDevice());

		//VertexBuffer

		m_VulkanVertexBuffer = Ref<VulkanVertexBuffer>::Create(sizeof(vertices[0]) * vertices.size());
		m_VulkanVertexBuffer->SetData((void*)vertices.data(), sizeof(vertices[0]) * vertices.size());

		//IndexBuffer
		m_VulkanIndexBuffer = Ref<VulkanIndexBuffer>::Create(sizeof(indices[0]) * indices.size());
		m_VulkanIndexBuffer->SetData((void*)indices.data(), sizeof(indices[0]) * indices.size());

		//UniformBuffer
		m_VulkanUniformBuffer.resize(m_Config.FramesInFlight);
		for (int i = 0; i < m_VulkanUniformBuffer.size(); i++)
		{
			m_VulkanUniformBuffer[i] = Ref<VulkanUniformBuffer>::Create(sizeof(UniformBufferObject));
		}

		//TODO MERGE TO MATERIAL

		// DescriptorLayout
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.pImmutableSamplers = nullptr;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		NX_CHECK_VKRESULT(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout));

		std::array<VkDescriptorPoolSize, 2> poolSizes{};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(m_Config.FramesInFlight);
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(m_Config.FramesInFlight);

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(m_Config.FramesInFlight);

		NX_CHECK_VKRESULT(vkCreateDescriptorPool(device, &poolInfo, nullptr, &m_UniformDescriptorPool));



		//layout
		VertexBufferLayout layout = {
			{ ShaderDataType::Float3,"inPosition"},
			{ ShaderDataType::Float3,"inColor"},
			{ ShaderDataType::Float2,"texCoord"},
		};

		//Shader
		auto shader = Ref<VulkanShader>::Create(device, "assets/shaders/vert.spv", "assets/shaders/frag.spv");
		VulkanPipeline::Config pipelineConfig(
			m_MainRenderPass->GetPass(),
			m_VulkanSwapChain->GetExtend2D(),
			shader,
			layout,
			descriptorSetLayout);

		m_VulkanPipeline->Set(pipelineConfig);

		//Pipeline End

		// Descriptor Sets

		std::vector<VkDescriptorSetLayout> layouts(m_Config.FramesInFlight, descriptorSetLayout);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_UniformDescriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(m_Config.FramesInFlight);
		allocInfo.pSetLayouts = layouts.data();

		descriptorSets.resize(m_Config.FramesInFlight);
		NX_CHECK_VKRESULT(vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()));

		for (size_t i = 0; i < m_Config.FramesInFlight; i++) {
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = m_VulkanUniformBuffer[i]->GetBuffer();
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = m_VulkanImage->GetImageView();
			imageInfo.sampler = m_VulkanImage->GetVkSampler();

			std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = descriptorSets[i];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = descriptorSets[i];
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}

	}

	void VulkanContext::ShutDown()
	{
		NX_CHECK_VKRESULT(vkDeviceWaitIdle(m_VulkanDevice->GetVkDevice()));
		/*cleanupSwapChain();

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroyBuffer(device, uniformBuffers[i], nullptr);
			vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
		}

		vkDestroyDescriptorPool(device, descriptorPool, nullptr);

		vkDestroyImage(device, textureImage, nullptr);
		vkFreeMemory(device, textureImageMemory, nullptr);

		vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

		vkDestroyBuffer(device, indexBuffer, nullptr);
		vkFreeMemory(device, indexBufferMemory, nullptr);

		vkDestroyBuffer(device, vertexBuffer, nullptr);
		vkFreeMemory(device, vertexBufferMemory, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(device, inFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(device, commandPool, nullptr);

		vkDestroyDevice(device, nullptr);

		if (enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		}*/
	}

	void VulkanContext::BeginFrame()
	{
		VkDevice device = m_VulkanDevice->GetVkDevice();
		VkSwapchainKHR swapChain = m_VulkanSwapChain->Get();

		NX_CHECK_VKRESULT(vkWaitForFences(device, 1, &allCompleteFences[m_CurFrameIndex], VK_TRUE, UINT64_MAX));
		NX_CHECK_VKRESULT(vkResetFences(device, 1, &allCompleteFences[m_CurFrameIndex]));

		m_VulkanSwapChain->AcquireNextImage(presentFinishedSemaphores[m_CurFrameIndex]);
		m_CurSwapChainImageIndex = m_VulkanSwapChain->GetCurImageIndex();

		m_VulkanUniformBuffer[m_CurFrameIndex]->Update();
		//if (vkImGui) { vkImGui->BeginFrame(); }
		m_VulkanDevice->ResetAll(m_CurFrameIndex);
	}

	void VulkanContext::DrawFrame()
	{
		//if (vkImGui) { vkImGui->OnRender(); }

		recordCommandBuffer(m_VulkanDevice->GetRenderCmdBuffer(m_CurFrameIndex), m_CurSwapChainImageIndex);
	}

	void VulkanContext::EndFrame()
	{
		//if (vkImGui) { vkImGui->EndFrame(); }
		auto renderCmdBuffer = m_VulkanDevice->GetRenderCmdBuffer(m_CurFrameIndex);

		VkDevice device = m_VulkanDevice->GetVkDevice();

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { presentFinishedSemaphores[m_CurFrameIndex] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &renderCmdBuffer;

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[m_CurFrameIndex] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		NX_CHECK_VKRESULT(vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, allCompleteFences[m_CurFrameIndex]));

		m_VulkanSwapChain->Present(signalSemaphores);

		m_CurFrameIndex = (m_CurFrameIndex + 1) % m_Config.FramesInFlight;
	}

	void VulkanContext::createVkInstance()
	{
		NX_CORE_ASSERT(glfwVulkanSupported(), "GLFW Don't Support Vulkan!");

		uint32_t supportVersion;
		NX_CHECK_VKRESULT(vkEnumerateInstanceVersion(&supportVersion));

		NX_CORE_ASSERT(supportVersion >= VK_API_VERSION_1_3, "Can't Support Vulkan Minimun Version!");

		// Create Instance Begin--------------------------------
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Next";
		appInfo.pEngineName = "Next";
		appInfo.apiVersion = VK_API_VERSION_1_3;

		// Check Validation Layer Support
		if (VulkanUtils::enableValidationLayers && !checkValidationLayerSupport()) {
			NX_CORE_ASSERT(false, "validation layers requested, but not available!");
		}

		auto extensions = getRequiredExtensions();

		VkInstanceCreateInfo instanceCreataInfo = {};
		instanceCreataInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreataInfo.pApplicationInfo = &appInfo;
		instanceCreataInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		instanceCreataInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
		if (VulkanUtils::enableValidationLayers) {
			debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debugCreateInfo.pfnUserCallback = VkDebugCallback;
			instanceCreataInfo.enabledLayerCount = static_cast<uint32_t>(VulkanUtils::validationLayers.size());
			instanceCreataInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
			instanceCreataInfo.ppEnabledLayerNames = VulkanUtils::validationLayers.data();
		}
		else {
			instanceCreataInfo.enabledLayerCount = 0;
		}

		NX_CHECK_VKRESULT(vkCreateInstance(&instanceCreataInfo, nullptr, &m_VkInstance));
	}

	void VulkanContext::setupDebugMessenger()
	{
		if (VulkanUtils::enableValidationLayers) {
			auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_VkInstance, "vkCreateDebugUtilsMessengerEXT");
			VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
			debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debugCreateInfo.pfnUserCallback = VkDebugCallback;
			NX_CHECK_VKRESULT(vkCreateDebugUtilsMessengerEXT(m_VkInstance, &debugCreateInfo, nullptr, &debugMessenger));
		}
	}

	bool VulkanContext::checkValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : VulkanUtils::validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	std::vector<const char*> VulkanContext::getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (VulkanUtils::enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	void VulkanContext::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		NX_CHECK_VKRESULT(vkBeginCommandBuffer(commandBuffer, &beginInfo));

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_MainRenderPass->GetPass();
		renderPassInfo.framebuffer = m_SwapChainFramebuffers[imageIndex]->Get();
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_VulkanSwapChain->GetExtend2D();

		VkClearValue clearColor = { {0.0f, 0.0f, 0.0f, 1.0f} };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_VulkanPipeline->GetVkPipeline());

		VkBuffer vertexBuffers[] = { m_VulkanVertexBuffer->GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(commandBuffer, m_VulkanIndexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT16);

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_VulkanPipeline->GetVkPipelineLayout(), 0, 1, &descriptorSets[m_CurFrameIndex], 0, nullptr);

		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

		vkCmdEndRenderPass(commandBuffer);

		NX_CHECK_VKRESULT(vkEndCommandBuffer(commandBuffer));

	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VkDebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			// Message is important enough to show
			NX_CORE_WARN("Vulkan WARNING : {0}", pCallbackData->pMessage);
		}
		else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			NX_CORE_ERROR("Vulkan ERROR : {0}", pCallbackData->pMessage);
		}
		else
		{
			//std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		}
		return VK_FALSE;
	}

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}
}

