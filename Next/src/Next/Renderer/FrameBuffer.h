#pragma once

#include "Next/Core/Ref.h"

namespace Next {

	class FrameBuffer : public RefCounted
	{
	public:
		virtual ~FrameBuffer() = default;
	};
}


