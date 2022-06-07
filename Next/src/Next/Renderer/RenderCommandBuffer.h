#pragma once

#include "Next/Core/Ref.h"

namespace Next {

	class RenderCommandBuffer : public RefCounted
	{
	public:
		virtual ~RenderCommandBuffer() = default;
		static Ref<RenderCommandBuffer> Create();
	};
}


