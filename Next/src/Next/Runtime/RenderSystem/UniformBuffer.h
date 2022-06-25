#pragma once

#include "Core/Ref.h"

#include <glm/glm.hpp>

namespace Next {

	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	class UniformBuffer : public RefCounted
	{
	public:
		UniformBuffer() = default;
		virtual ~UniformBuffer() = default;

		virtual void Update() = 0;
		virtual void Release() = 0;
	};

}

