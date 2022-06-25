#pragma once

#include "Runtime/RenderSystem/Image.h"

#include "VulkanUtils.h"
#include "VulkanAllocator.h"


namespace Next {

	// Include 2 types : 
	// File Image
	// Depth Image
	class VulkanImage : public Image
	{
	public:
		VulkanImage();
		virtual ~VulkanImage() override;

		//used as a normal file image
		void LoadByFile(const std::string& filePath);

		//uesd as as Depth Resources
		void CreateDepthResource();

		inline VkImageView GetImageView() const { return m_ImageView; }
		inline VkSampler GetVkSampler() const { return m_TextureSampler; }

	private:
		VkImage m_Image;
		VkImageView m_ImageView;
		//Can be used on different places
		VkSampler m_TextureSampler;
		uint32_t mipLevels;

		VmaAllocation m_Allocation = nullptr;
	
	private:
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

		bool hasStencilComponent(VkFormat format);
	};
}


