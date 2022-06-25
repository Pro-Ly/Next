#pragma once

#include <string>

namespace Next {

	struct WindowSpecification
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowSpecification(const std::string& title = "Next Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	enum class RendererAPIType
	{
		None,
		Vulkan,
	};

	struct RendererConfig
	{
		RendererAPIType RendererApi;
		uint32_t FramesInFlight;
	};

	struct GameEngineSpecification
	{
		WindowSpecification windowSpec;
		RendererConfig rendererConfig;
	};


}