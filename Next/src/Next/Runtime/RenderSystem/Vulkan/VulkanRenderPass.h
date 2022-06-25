#pragma once

#include "Core/Ref.h"
#include "VulkanUtils.h"

namespace Next {

	class VulkanRenderPass : public RefCounted
	{
	public:
		VulkanRenderPass(VkSurfaceFormatKHR surfaceFormat);
		~VulkanRenderPass();

		inline const VkRenderPass GetPass() const { return m_VkRenderPass; }
	private:
		VkRenderPass m_VkRenderPass;

		VkSurfaceFormatKHR m_SurfaceFormat;
	};
}


