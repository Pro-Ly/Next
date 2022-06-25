#pragma once

#include "ImGuiBase.h"

#include "Runtime/RenderSystem/vulkan/VulkanUtils.h"
#include <GLFW/glfw3.h>
#include "Runtime/RenderSystem/Vulkan/VulkanRenderer.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include <vector>

namespace Next {

	class EditorLayer;

	class VulkanImGui final : public ImGuiBase
	{
	public:
		VulkanImGui(EditorLayer* editorLayer);
		virtual ~VulkanImGui() override = default;

		virtual void Init() override {};
		virtual void BeginFrame() override {};
		virtual void EndFrame() override {};
		virtual void Destroy() override;
		virtual void OnRender() override;
	private:

		void SetUIColorStyle();
	private:
		VulkanRenderer* m_vkAPI = nullptr;
		EditorLayer* m_EditorLayer;
		std::vector<VkCommandBuffer> m_CommandBuffers;
	};

}

