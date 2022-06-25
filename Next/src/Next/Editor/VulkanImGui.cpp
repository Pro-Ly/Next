#include "nxpch.h"
#include "VulkanImGui.h"
#include "Next/Core/Application.h"
#include "Runtime/RenderSystem/RenderSystem.h"
#include "backends/imgui_impl_vulkan.cpp"
#include "backends/imgui_impl_glfw.cpp"
#include "Utilities/ImGuiUtilities.h"

#include <imgui.h>

#define IMGUI_VULKAN_DEBUG_REPORT
namespace Next {

	void CheckImGuiVkResult(VkResult res)
	{
		NX_CHECK_VKRESULT(res);
	}

	VulkanImGui::VulkanImGui(EditorLayer* editorLayer)
		:m_EditorLayer(editorLayer)
	{
	/*	auto& io = ImGui::GetIO();
		io.AddMouseButtonEvent()*/
	}

	//void VulkanImGui::Init()
	//{
	//	m_vkAPI = (VulkanRendererAPI*)RenderSystem::s_RendererAPI;
	//	m_vkAPI->vkImGui = this;
	//	GLFWwindow* window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();

	//	// Setup Dear ImGui context
	//	IMGUI_CHECKVERSION();
	//	ImGui::CreateContext();
	//	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//	//io.ConfigViewportsNoDecoration = false;
	//	//io.ConfigViewportsNoAutoMerge = true;
	//	//io.ConfigViewportsNoTaskBarIcon = true;

	//	// Setup Dear ImGui style
	//	ImGui::StyleColorsDark();
	//	ImGui::StyleColorsClassic();

	//	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	//	ImGuiStyle& style = ImGui::GetStyle();
	//	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	//	{
	//		style.WindowRounding = 0.0f;
	//		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	//	}
	//	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, style.Colors[ImGuiCol_WindowBg].w);

	//	VkDescriptorPool descriptorPool;

	//	// Create Descriptor Pool
	//	VkDescriptorPoolSize pool_sizes[] =
	//	{
	//		{ VK_DESCRIPTOR_TYPE_SAMPLER, 100 },
	//		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100 },
	//		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100 },
	//		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 100 },
	//		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 100 },
	//		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 100 },
	//		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100 },
	//		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 100 },
	//		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 100 },
	//		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 100 },
	//		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 100 }
	//	};
	//	VkDescriptorPoolCreateInfo pool_info = {};
	//	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	//	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	//	pool_info.maxSets = 100 * IM_ARRAYSIZE(pool_sizes);
	//	pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
	//	pool_info.pPoolSizes = pool_sizes;
	//	NX_CHECK_VKRESULT(vkCreateDescriptorPool(m_vkAPI->device, &pool_info, nullptr, &descriptorPool));

	//	// Setup Platform/Renderer bindings
	//	ImGui_ImplGlfw_InitForVulkan(window, true);
	//	ImGui_ImplVulkan_InitInfo init_info = {};
	//	init_info.Instance = m_vkAPI->m_VkInstance;
	//	init_info.PhysicalDevice = m_vkAPI->physicalDevice;
	//	init_info.Device = m_vkAPI->device;
	//	init_info.QueueFamily = m_vkAPI->queueFamilyIndices.graphicsFamily.value();
	//	init_info.Queue = m_vkAPI->graphicsQueue;
	//	init_info.PipelineCache = nullptr;
	//	init_info.DescriptorPool = descriptorPool;
	//	init_info.Allocator = nullptr;
	//	init_info.MinImageCount = 2;
	//	init_info.ImageCount = m_vkAPI->swapChainImageCount;
	//	init_info.CheckVkResultFn = CheckImGuiVkResult;

	//	ImGui_ImplVulkan_Init(&init_info, m_vkAPI->m_MainRenderPass);

	//	// Load Fonts
	//		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	//		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	//		// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	//		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	//		// - Read 'docs/FONTS.md' for more instructions and details.
	//		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//		//io.Fonts->AddFontDefault();
	//		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//		//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//		//IM_ASSERT(font != NULL);

	//		// Upload Fonts
	//	{
	//		// Use any command queue

	//		VkCommandBuffer commandBuffer = m_vkAPI->CreateCommandBuffer();
	//		VkCommandBufferBeginInfo cmdBufferBeginInfo{};
	//		cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//		NX_CHECK_VKRESULT(vkBeginCommandBuffer(commandBuffer, &cmdBufferBeginInfo));
	//		ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
	//		const uint64_t DEFAULT_FENCE_TIMEOUT = 100000000000;

	//		NX_CHECK_VKRESULT(vkEndCommandBuffer(commandBuffer));

	//		VkSubmitInfo submitInfo = {};
	//		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	//		submitInfo.commandBufferCount = 1;
	//		submitInfo.pCommandBuffers = &commandBuffer;

	//		// Create fence to ensure that the command buffer has finished executing
	//		VkFenceCreateInfo fenceCreateInfo = {};
	//		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	//		fenceCreateInfo.flags = 0;
	//		VkFence fence;
	//		NX_CHECK_VKRESULT(vkCreateFence(m_vkAPI->device, &fenceCreateInfo, nullptr, &fence));

	//		// Submit to the queue
	//		NX_CHECK_VKRESULT(vkQueueSubmit(m_vkAPI->graphicsQueue, 1, &submitInfo, fence));
	//		// Wait for the fence to signal that command buffer has finished executing
	//		NX_CHECK_VKRESULT(vkWaitForFences(m_vkAPI->device, 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT));

	//		vkDestroyFence(m_vkAPI->device, fence, nullptr);
	//		vkFreeCommandBuffers(m_vkAPI->device, m_vkAPI->defaultCommandPool, 1, &commandBuffer);

	//		NX_CHECK_VKRESULT(vkDeviceWaitIdle(m_vkAPI->device));
	//		ImGui_ImplVulkan_DestroyFontUploadObjects();
	//	}

	//	uint32_t framesInFlight = RenderSystem::GetConfig().FramesInFlight;
	//	m_CommandBuffers.resize(framesInFlight);
	//	for (uint32_t i = 0; i < framesInFlight; i++)
	//		m_CommandBuffers[i] = m_vkAPI->CreateSecondaryCommandBuffer();

	//}

	//void VulkanImGui::BeginFrame()
	//{
	//	ImGui_ImplVulkan_NewFrame();
	//	ImGui_ImplGlfw_NewFrame();
	//	ImGui::NewFrame();
	//}

	//void VulkanImGui::EndFrame()
	//{
	//	ImGui::Render();

	//	VkClearValue clearValues[2];
	//	clearValues[0].color = { {0.1f, 1.0f,0.1f, 0.0f} };
	//	clearValues[1].depthStencil = { 1.0f, 0 };

	//	uint32_t width = m_vkAPI->m_Width;
	//	uint32_t height = m_vkAPI->m_Height;

	//	uint32_t commandBufferIndex = m_vkAPI->currentFrameIndex;

	//	VkCommandBufferBeginInfo drawCmdBufInfo = {};
	//	drawCmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//	drawCmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	//	drawCmdBufInfo.pNext = nullptr;

	//	VkCommandBuffer drawCommandBuffer = m_vkAPI->renderCommandBuffers[commandBufferIndex];
	//	NX_CHECK_VKRESULT(vkBeginCommandBuffer(drawCommandBuffer, &drawCmdBufInfo));

	//	VkRenderPassBeginInfo renderPassBeginInfo = {};
	//	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	//	renderPassBeginInfo.pNext = nullptr;
	//	renderPassBeginInfo.renderPass = m_vkAPI->m_MainRenderPass;
	//	renderPassBeginInfo.renderArea.offset.x = 0;
	//	renderPassBeginInfo.renderArea.offset.y = 0;
	//	renderPassBeginInfo.renderArea.extent.width = width;
	//	renderPassBeginInfo.renderArea.extent.height = height;
	//	renderPassBeginInfo.clearValueCount = 2; // Color + depth
	//	renderPassBeginInfo.pClearValues = clearValues;
	//	renderPassBeginInfo.framebuffer = m_vkAPI->m_SwapChainFramebuffers[commandBufferIndex];

	//	vkCmdBeginRenderPass(drawCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);

	//	VkCommandBufferInheritanceInfo inheritanceInfo = {};
	//	inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
	//	inheritanceInfo.renderPass = m_vkAPI->m_MainRenderPass;
	//	inheritanceInfo.framebuffer = m_vkAPI->m_SwapChainFramebuffers[commandBufferIndex];

	//	VkCommandBufferBeginInfo cmdBufInfo = {};
	//	cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//	cmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
	//	cmdBufInfo.pInheritanceInfo = &inheritanceInfo;

	//	NX_CHECK_VKRESULT(vkBeginCommandBuffer(m_CommandBuffers[commandBufferIndex], &cmdBufInfo));

	//	VkViewport viewport = {};
	//	viewport.x = 0.0f;
	//	viewport.y = (float)height;
	//	viewport.height = -(float)height;
	//	viewport.width = (float)width;
	//	viewport.minDepth = 0.0f;
	//	viewport.maxDepth = 1.0f;
	//	vkCmdSetViewport(m_CommandBuffers[commandBufferIndex], 0, 1, &viewport);

	//	VkRect2D scissor = {};
	//	scissor.extent.width = width;
	//	scissor.extent.height = height;
	//	scissor.offset.x = 0;
	//	scissor.offset.y = 0;
	//	vkCmdSetScissor(m_CommandBuffers[commandBufferIndex], 0, 1, &scissor);

	//	ImDrawData* main_draw_data = ImGui::GetDrawData();
	//	ImGui_ImplVulkan_RenderDrawData(main_draw_data, m_CommandBuffers[commandBufferIndex]);

	//	NX_CHECK_VKRESULT(vkEndCommandBuffer(m_CommandBuffers[commandBufferIndex]));

	//	std::vector<VkCommandBuffer> commandBuffers;
	//	commandBuffers.push_back(m_CommandBuffers[commandBufferIndex]);

	//	vkCmdExecuteCommands(drawCommandBuffer, uint32_t(commandBuffers.size()), commandBuffers.data());

	//	vkCmdEndRenderPass(drawCommandBuffer);

	//	NX_CHECK_VKRESULT(vkEndCommandBuffer(drawCommandBuffer));

	//	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//	// Update and Render additional Platform Windows
	//	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	//	{
	//		ImGui::UpdatePlatformWindows();
	//		ImGui::RenderPlatformWindowsDefault();
	//	}
	//}

	void VulkanImGui::SetUIColorStyle()
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		//colors[ImGuiCol_Text] = ImVec4(0.4745f, 0.4745f, 0.4745f, 1.00f);
	}


	void VulkanImGui::Destroy()
	{

	}

	void VulkanImGui::OnRender()
	{
		bool show = true;
		ImGui::ShowDemoWindow(&show);
		//// ImGui + Dockspace Setup ------------------------------------------------------------------------------
		//ImGuiIO& io = ImGui::GetIO();
		//ImGuiStyle& style = ImGui::GetStyle();
		////auto boldFont = io.Fonts->Fonts[0];
		////auto largeFont = io.Fonts->Fonts[1];

		//if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || (ImGui::IsMouseClicked(ImGuiMouseButton_Right)))
		//{
		//	ImGui::FocusWindow(GImGui->HoveredWindow);
		//	//Input::SetCursorMode(CursorMode::Normal);
		//}

		//io.ConfigWindowsResizeFromEdges = io.BackendFlags & ImGuiBackendFlags_HasMouseCursors;

		//// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		//// because it would be confusing to have two docking targets within each others.
		//ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

		//ImGuiViewport* viewport = ImGui::GetMainViewport();
		//ImGui::SetNextWindowPos(viewport->Pos);
		//ImGui::SetNextWindowSize(viewport->Size);
		//ImGui::SetNextWindowViewport(viewport->ID);
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		//window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		//window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		//auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		//bool isMaximized = (bool)glfwGetWindowAttrib(window, GLFW_MAXIMIZED);

		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, isMaximized ? ImVec2(6.0f, 6.0f) : ImVec2(1.0f, 1.0f));
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);
		//ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
		////ImGui::Begin("DockSpace Demo", nullptr, window_flags);
		//ImGui::PopStyleColor(); // MenuBarBg
		//ImGui::PopStyleVar(2);

		//ImGui::PopStyleVar(2);

		//{
		//	UI::ScopedColour windowBorder(ImGuiCol_Border, IM_COL32(50, 50, 50, 255));
		//	// Draw window border if the window is not maximized
		//	if (!isMaximized)
		//		UI::RenderWindowOuterBorders(ImGui::GetCurrentWindow());
		//}

		//const bool maximized = (bool)glfwGetWindowAttrib(window, GLFW_MAXIMIZED);

		//ImVec2 newSize, newPosition;
		//if (!maximized && UI::UpdateWindowManualResize(ImGui::GetCurrentWindow(), newSize, newPosition))
		//{
		//	// On Windows we hook into the GLFW win32 window internals
		//	#ifndef HZ_PLATFORM_WINDOWS

		//	glfwSetWindowPos(window, newPosition.x, newPosition.y);
		//	glfwSetWindowSize(window, newSize.x, newSize.y);
		//	#endif
		//}

		////const float titlebarHeight = UI_DrawTitlebar();
		////ImGui::SetCursorPosY(titlebarHeight + ImGui::GetCurrentWindow()->WindowPadding.y);

		//// Dockspace
		//float minWinSizeX = style.WindowMinSize.x;
		//style.WindowMinSize.x = 370.0f;
		//ImGui::DockSpace(ImGui::GetID("MyDockspace"));
		//style.WindowMinSize.x = minWinSizeX;

		//// Editor Panel ------------------------------------------------------------------------------
		////ImGui::ShowDemoWindow();

		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		//ImGui::Begin("Viewport");
		//{
		//	//m_ViewportPanelMouseOver = ImGui::IsWindowHovered();
		//	//m_ViewportPanelFocused = ImGui::IsWindowFocused();

		//	//Application::Get().GetImGuiLayer()->AllowInputEvents(!(m_ViewportPanelMouseOver && Input::IsMouseButtonPressed(MouseButton::Right)));

		//	//auto viewportOffset = ImGui::GetCursorPos(); // includes tab bar
		//	//auto viewportSize = ImGui::GetContentRegionAvail();
		//	//m_ViewportRenderer->SetViewportSize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		//	//m_EditorScene->SetViewportSize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		//	//if (m_RuntimeScene)
		//	//	m_RuntimeScene->SetViewportSize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		//	//m_EditorCamera.SetViewportSize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);

		//	//// Render viewport image
		//	//UI::Image(m_ViewportRenderer->GetFinalPassImage(), viewportSize, { 0, 1 }, { 1, 0 });

		//	//auto windowSize = ImGui::GetWindowSize();
		//	//ImVec2 minBound = ImGui::GetWindowPos();

		//	//// NOTE(Peter): This currently just subtracts 0 because I removed the toolbar window, but I'll keep it in just in case
		//	//minBound.x -= viewportOffset.x;
		//	//minBound.y -= viewportOffset.y;

		//	//ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		//	//m_ViewportBounds[0] = { minBound.x, minBound.y };
		//	//m_ViewportBounds[1] = { maxBound.x, maxBound.y };

		//	//m_AllowViewportCameraEvents = (ImGui::IsMouseHoveringRect(minBound, maxBound) && m_ViewportPanelFocused) || m_StartedRightClickInViewport;

		//	//UI_GizmosToolbar();
		//	//UI_CentralToolbar();
		//	//UI_ViewportSettings();

		//	//if (m_CurrentScene != m_RuntimeScene)
		//	//	UI_DrawGizmos(m_EditorCamera);

		//	//UI_HandleAssetDrop();
		//}
		//ImGui::End();
		//ImGui::PopStyleVar();
	}
}
