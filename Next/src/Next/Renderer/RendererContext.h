#pragma once

#include "Next/Core/Ref.h"

struct  GLFWwindow;

namespace Next {

	class RendererContext : public RefCounted
	{
		friend class Ref<RendererContext>;
	protected:
		RendererContext() = default;
		virtual ~RendererContext() = default;
	public:
		virtual void Init() = 0;
		/*virtual void BeginFrame() = 0;
		virtual void SwapBuffers() = 0;

		virtual void OnResize(uint32_t width, uint32_t height) = 0;*/

		static Ref<RendererContext> Create(GLFWwindow* windowHandle);
	};

}


