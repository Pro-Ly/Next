#pragma once

#include "VulkanUtils.h"
#include "Core/Ref.h"
#include "VulkanVertexBuffer.h"
#include "VulkanShader.h"

#include <glm/glm.hpp>

namespace Next {

	class VulkanPipeline : public RefCounted
	{
	public:
		struct Config
		{
			VkRenderPass renderPass;
			VkExtent2D extend2D;
			Ref<VulkanShader> vulkanShader;
			VertexBufferLayout vertexBufferLayout;
			VkDescriptorSetLayout vkDescriptorSetLayout;
		};

		VulkanPipeline();
		~VulkanPipeline();
		void Set(Config config);
		inline const VkPipeline GetVkPipeline() const { return m_VkPipeline; }
		inline const VkPipelineLayout GetVkPipelineLayout() const { return m_PipelineLayout; }
		inline const Config GetConfig() const { return m_Config; }
	private:
		VkPipeline m_VkPipeline;
		VkPipelineCache pipelineCache;
		VkPipelineLayout m_PipelineLayout;

		Config m_Config;
	private:
		void saveGraphicsPiplineCache(VkDevice device, VkPipelineCache pipelineCache);
	};
}


