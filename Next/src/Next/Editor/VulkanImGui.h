#pragma once

#include "ImGuiBase.h"

#include <vulkan/vulkan.h>

namespace Next {

	class VulkanImGui final : public ImGuiBase
	{
		struct ImGui_ImplVulkan_InitInfo
		{
			VkInstance                      Instance;
			VkPhysicalDevice                PhysicalDevice;
			VkDevice                        Device;
			uint32_t                        QueueFamily;
			VkQueue                         Queue;
			VkPipelineCache                 PipelineCache;
			VkDescriptorPool                DescriptorPool;
			uint32_t                        Subpass;
			uint32_t                        MinImageCount;          // >= 2
			uint32_t                        ImageCount;             // >= MinImageCount
			VkSampleCountFlagBits           MSAASamples;            // >= VK_SAMPLE_COUNT_1_BIT (0 -> default to VK_SAMPLE_COUNT_1_BIT)
			const VkAllocationCallbacks* Allocator;
			void                            (*CheckVkResultFn)(VkResult err);
		};

	public:
		VulkanImGui() = default;
		virtual ~VulkanImGui() override = default;

		virtual void Init() override;
		virtual void Destroy() override;
		virtual void BeginFrame() override;
		virtual void EndFrame() override;
	};

}

