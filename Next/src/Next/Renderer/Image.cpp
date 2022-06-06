#include "nxpch.h"
#include "Image.h"
#include "Next/Renderer/RendererAPI.h"
#include "Next/Platform/Vulkan/VulkanImage.h"

namespace Next {


	//Ref<Image2D> Image2D::Create(ImageSpecification specification)
	//{
	//	switch (RendererAPI::GetCurrentType())
	//	{
	//		case RendererAPIType::None: return nullptr;
	//		case RendererAPIType::Vulkan: return Ref<VulkanImage>::Create(specification);
	//	}
	//	NX_CORE_ASSERT(false, "Unknown RendererAPI");
	//	return nullptr;
	//}
}