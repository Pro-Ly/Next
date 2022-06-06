#pragma once

#include "Next/Core/Ref.h"

#include <glm/glm.hpp>

namespace Next {

	struct ImageSpecification
	{
		/*	std::string DebugName;

			ImageFormat Format = ImageFormat::RGBA;
			ImageUsage Usage = ImageUsage::Texture;
			uint32_t Width = 1;
			uint32_t Height = 1;
			uint32_t Mips = 1;
			uint32_t Layers = 1;*/
	};

	class Image : public RefCounted
	{
	public:
		virtual ~Image() = default;

		virtual void Resize(const uint32_t width, const uint32_t height) = 0;
		virtual void Invalidate() = 0;
		virtual void Release() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual glm::uvec2 GetSize() const = 0;

		virtual float GetAspectRatio() const = 0;

		virtual ImageSpecification& GetSpecification() = 0;
		virtual const ImageSpecification& GetSpecification() const = 0;

		//virtual Buffer GetBuffer() const = 0;
		//virtual Buffer& GetBuffer() = 0;

		virtual void CreatePerLayerImageViews() = 0;

		virtual uint64_t GetHash() const = 0;

		// TODO: usage (eg. shader read)
	};

	class Image2D : public Image
	{
	public:
		/*static Ref<Image2D> Create(ImageSpecification specification);*/
		virtual void Resize(const glm::uvec2& size) = 0;
	};
}