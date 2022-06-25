#pragma once

#include "Next/Core/Events/KeyEvent.h"
#include "Next/Core/Events/MouseEvent.h"
#include "Next/Core/Events/ApplicationEvent.h"
#include "Next\Core\Layer.h"
#include "ImGuiBase.h"

namespace Next {

	class EngineLayer;

	class NEXT_API EditorLayer :public Layer
	{
	public:
		EditorLayer(EngineLayer& scenelayer);
		~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Next::Timestep timestep) override;
		virtual void OnEvent(Event& event) override;
		void OnImGuiRender();
	private:
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnWindowCloseEvent(WindowCloseEvent& e);
	private:
		std::unique_ptr<ImGuiBase> m_ImGui;
		EngineLayer& m_SceneLayer;
		float m_Time = 0.0f;

		int FPS = 0;
	};
}