#pragma once

#include "Core.h"

#include "Window.h"
#include "Next\LayerStack.h"
#include "Next\Events\Event.h"
#include "Next\Events\ApplicationEvent.h"

#include "Next/Core/Timestep.h"

#include "Next\ImGui\ImGuiLayer.h"

namespace Next {

	class NEXT_API  Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();

}

