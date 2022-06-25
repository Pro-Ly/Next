#pragma once

#include "Core/Ref.h"
#include "VulkanUtils.h"

namespace Next {

	class VulkanRenderPass : public RefCounted
	{
	public:
		VulkanRenderPass(VkDevice device, VkSurfaceFormatKHR surfaceFormat);
		~VulkanRenderPass();

		inline const VkRenderPass GetPass() const { return m_VkRenderPass; }
	private:
		VkRenderPass m_VkRenderPass;

		VkDevice m_VkDevice;
		VkSurfaceFormatKHR m_SurfaceFormat;
	};
}


