#pragma once
#include "Next/Renderer/Pipeline.h"
#include "vulkan/vulkan.h"

namespace Next {

	class VulkanPipeline : public Pipeline
	{
	public:
		VulkanPipeline(const PipelineSpecification& spec);
		virtual ~VulkanPipeline() override;
		inline VkPipeline GetGraphicsPipeline() const { return m_GraphicsPipeline; }
	private:
		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_GraphicsPipeline;
	};
}


