#pragma once

extern struct GLFWwindow;

namespace Next {

	enum class RendererAPIType
	{
		None,
		Vulkan,
	};

	class RendererAPI
	{
	public:
		RendererAPI() = default;
		virtual ~RendererAPI() = default;
		virtual void Init(GLFWwindow* window) = 0;
		virtual void Shutdown() = 0;
		virtual void drawFrame() = 0;
		static RendererAPI* Create(RendererAPIType apiType);
		inline const static RendererAPIType GetCurrentAPIType() { return s_APIType; }
	private:
		static RendererAPIType s_APIType;
	};
}