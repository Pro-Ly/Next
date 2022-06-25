#pragma once

#include "Runtime/RenderSystem/Image.h"

#include "VulkanUtils.h"
#include "VulkanAllocator.h"


namespace Next {

	class VulkanImage : public Image
	{
	public:
		VulkanImage(const std::string& filePath);
		virtual ~VulkanImage() override;

		inline VkImageView GetImageView() const { return m_TextureImageView; }
		inline VkSampler GetVkSampler() const { return m_TextureSampler; }

	private:
		VkImage m_TextureImage;
		VkImageView m_TextureImageView;
		//Can be used on different places
		VkSampler m_TextureSampler;

		VmaAllocation m_Allocation;
	
	private:
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	};
}


