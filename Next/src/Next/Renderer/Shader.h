#pragma once

#include <string>
#include "Next/Core/Ref.h"

namespace Next {

	class Shader : public RefCounted
	{
	public:
		static Ref<Shader> Create(const std::string& filePath);
		static Ref<Shader> CreateFromString(const std::string& shaderSrc);
	};
}


