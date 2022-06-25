#pragma once
#include "Core/Config.h"
#include "Core/Ref.h"

namespace Next {

	class RendererAPI
	{
	public:
		RendererAPI() = default;
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void ShutDown() = 0;
		virtual void BeginFrame() = 0;
		virtual void drawFrame() = 0;
		virtual void EndFrame() = 0;

		static RendererAPI* Create(RendererAPIType apiType);
	};
}


