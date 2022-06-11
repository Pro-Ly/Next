#pragma once

#include "Next\Core\Layer.h"

namespace Next {

	class NEXT_API EditorLayer :public Layer
	{
	public:
		EditorLayer();
		~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Next::Timestep timestep) override;
		virtual void OnImGuiRender() override;
	private:
		float m_Time = 0.0f;
	};
}