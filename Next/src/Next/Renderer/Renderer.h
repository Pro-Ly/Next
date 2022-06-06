#pragma once

#include "Next/Core/Ref.h"
#include "RendererContext.h"
#include "Next/Application.h"

namespace Next {

	class Renderer : public RefCounted
	{
	public:
		inline static Ref<RendererContext> GetContext() { return Application::Get().GetWindow().GetRenderContext(); }
	};
}


