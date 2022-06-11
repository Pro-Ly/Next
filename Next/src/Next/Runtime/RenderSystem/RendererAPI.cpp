#include "nxpch.h"
#include "RendererAPI.h"
#include "Core/Core.h"
#include "Runtime/Platform/Vulkan/VulkanRendererAPI.h"

namespace Next {

	RendererAPIType RendererAPI::s_APIType = RendererAPIType::None;

	RendererAPI* RendererAPI::Create(RendererAPIType apiType)
	{
		switch (apiType)
		{
			case RendererAPIType::None:
				NX_CORE_ASSERT(false, "Cannot Create None API Type!!")
				return nullptr;
			case RendererAPIType::Vulkan:
				NX_CORE_ASSERT(s_APIType != apiType,"Already Create APIType !!")
				s_APIType = apiType;
				return new VulkanRendererAPI();
		}
		NX_CORE_ASSERT(false, "Unknown API Type!!")
		return nullptr;
	}

}