#pragma once

#include "Next\Layer.h"

#include "Next\Events\KeyEvent.h"
#include "Next\Events\ApplicationEvent.h"
#include "Next\Events\MouseEvent.h"

namespace Next {

	class NEXT_API ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Next::Timestep timestep) override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}