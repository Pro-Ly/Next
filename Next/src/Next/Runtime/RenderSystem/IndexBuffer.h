#pragma once

#include "Core/Ref.h"

namespace Next {

	class IndexBuffer : public RefCounted
	{
	public:
		IndexBuffer() = default;
		virtual ~IndexBuffer() = default;

		virtual void SetData(void* pData, size_t size, uint32_t bufferPosOffset = 0) = 0;
		virtual void Release() = 0;
	};
}


