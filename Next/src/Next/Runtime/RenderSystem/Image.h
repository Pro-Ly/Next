#pragma once

#include "Core/Ref.h"


namespace Next {

	class Image : public RefCounted
	{
	public:
		Image() = default;
		virtual ~Image() = default;
	};
}

