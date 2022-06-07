#pragma once

#include "Next/Core/Ref.h"
#include "Next/Renderer/RenderCommandBuffer.h"

namespace Next {

	class SceneRenderer : public RefCounted
	{
	public:
		SceneRenderer();
		~SceneRenderer();
	private:
		Ref<RenderCommandBuffer> m_CommandBuffer;
	};
}


