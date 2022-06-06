#pragma once

#include "Next/Core/Ref.h"

namespace Next {

	struct PipelineSpecification
	{

	};

	class Pipeline : public RefCounted
	{
	public:
		Pipeline() = default;
		virtual ~Pipeline() {};
		static Ref<Pipeline> Create(const PipelineSpecification& spec);
	};
}