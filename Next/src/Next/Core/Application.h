#pragma once

#include "Core.h"
#include "Config.h"

#include "Window.h"
#include "LayerStack.h"
#include "Events\Event.h"
#include "Events\ApplicationEvent.h"

#include "Timestep.h"

#include "Next/Editor/EditorLayer.h"
#include "Next/Editor\ImGuiBase.h"

namespace Next {
	class NEXT_API Application
	{
	public:
		Application(const GameEngineSpecification& engineSpec);
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
		inline static bool IsMinimized() { return Get().m_Minimized; }
	private:
		void StartWindow(const WindowSpecification& spec);
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;

		EditorLayer* m_EditorLayer;
		LayerStack m_LayerStack;


		bool m_Running = true;
		bool m_Minimized = false;		
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();

}

