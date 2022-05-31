#pragma once


namespace Next {

	enum class RendererAPIType
	{
		None,
		Vulkan,
	};

	class RendererAPI
	{
	public:
		inline static RendererAPIType GetCurrentType() { return s_CurRendererAPIType; }
	private:
		inline static RendererAPIType s_CurRendererAPIType = RendererAPIType::Vulkan;//Only for now
	};
}