#include "nxpch.h"

#include "Pipeline.h"
#include "Next/Platform/Vulkan/VulkanPipeline.h"
#include "RendererAPI.h"

namespace Next {


	Ref<Pipeline> Pipeline::Create(const PipelineSpecification& spec)
	{
		switch (RendererAPI::GetCurrentType())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::Vulkan:  return Ref<VulkanPipeline>::Create(spec);
		}
		NX_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}