#pragma once

#include "Core/Ref.h"
#include "VulkanUtils.h"

namespace Next {

	class VulkanFrameBuffer : public RefCounted
	{
	public:
		struct InitInfo
		{
			VkDevice vkDevice;
			VkRenderPass vkRenderPass;
			VkImageView vkImageView;
			uint32_t width;
			uint32_t height;
		};

		VulkanFrameBuffer(InitInfo initInfo);
		~VulkanFrameBuffer();

		inline const VkFramebuffer Get() const { return m_VkFrameBuffer; }
	private:
		InitInfo m_InitInfo;

		VkFramebuffer m_VkFrameBuffer;
	};


}

