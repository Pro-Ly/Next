#include "nxpch.h"
#include "VulkanPipeline.h"
#include "Core/Utilities/FileUtils.h"
#include "Core/Core.h"

#include <ostream>

namespace Next {


	VulkanPipeline::VulkanPipeline(VkDevice vkDevice)
		:m_VkDevice(vkDevice)
	{
		VkPipelineCacheCreateInfo cacheInfo{};
		cacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		cacheInfo.pNext = nullptr;
		// Read From File
		//cacheInfo.initialDataSize = ;
		//cacheInfo.pInitialData = ;

		NX_CHECK_VKRESULT(vkCreatePipelineCache(m_VkDevice, &cacheInfo, nullptr, &pipelineCache));
	}

	VulkanPipeline::~VulkanPipeline()
	{

	}

	void VulkanPipeline::Set(Config config)
	{
		m_Config = config;

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = config.vertexBufferLayout.GetStride();
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(config.vertexBufferLayout.GetElementCount());

		uint32_t i = 0;
		for (const auto& element : config.vertexBufferLayout.GetElements())
		{
			attributeDescriptions[i].binding = 0;
			attributeDescriptions[i].location = i;
			attributeDescriptions[i].format = ShaderDataTypeToVulkanFormat(element.Type);
			attributeDescriptions[i].offset = element.Offset;
			i++;
		}

		auto shaderStageCreateInfos = config.vulkanShader->GetShaderStageCreateInfos();

		//auto& pSetLayout = config.vkDescriptorSetLayout;

		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = (float)config.extend2D.height;
		viewport.width = (float)config.extend2D.width;
		viewport.height = -(float)config.extend2D.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = config.extend2D;

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &config.vkDescriptorSetLayout;

		NX_CHECK_VKRESULT(vkCreatePipelineLayout(m_VkDevice, &pipelineLayoutInfo, nullptr, &m_PipelineLayout));

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = shaderStageCreateInfos.size();
		pipelineInfo.pStages = shaderStageCreateInfos.data();
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.layout = m_PipelineLayout;
		pipelineInfo.renderPass = config.renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		NX_CHECK_VKRESULT(vkCreateGraphicsPipelines(m_VkDevice, pipelineCache, 1, &pipelineInfo, nullptr, &m_VkPipeline));

		config.vulkanShader->Release();
		saveGraphicsPiplineCache(m_VkDevice, pipelineCache);
	}

	void VulkanPipeline::saveGraphicsPiplineCache(VkDevice device, VkPipelineCache pipelineCache)
	{
		//size_t cacheSize = 0;

		//NX_CORE_ASSERT(vkGetPipelineCacheData(device, pipelineCache, &cacheSize, nullptr),"getting cache size fail from pipelinecache")

		//auto cacheData = std::vector<char>(sizeof(char) * cacheSize, 0);

		//NX_CORE_ASSERT(vkGetPipelineCacheData(device, pipelineCache, &cacheSize, &cacheData[0]),"getting cache fail from pipelinecache");

		//std::ofstream stream(vkConfig.pipelineCacheFile, std::ios::binary);
		//if (stream.is_open()) {
		//	stream.write(cacheData.data(), cacheData.size());
		//	stream.close();
		//}
		//else {
		//	std::cerr << "open pipeline cache data target file failed!" << std::endl;
		//	return;
		//}
	}

}