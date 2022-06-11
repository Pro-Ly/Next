#include "nxpch.h"
#include "Application.h"

#include "Next\Core\Log.h"
#include "Core.h"

#include "Input.h"
#include "Runtime/RenderSystem/RenderSystem.h"
#include "Runtime/RenderSystem/SceneRenderer.h"
#include "Editor/VulkanImGui.h"

#include <GLFW/glfw3.h>

namespace Next {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		NX_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		RenderSystem::Select(RendererAPIType::Vulkan);

		// -------------------------------------------------------------------
		// Create Window With Vulkan ( For Now )
		// -------------------------------------------------------------------
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->Init();
		m_Window->SetEventCallback(NX_BIND_EVENT_FN(Application::OnEvent));

		RenderSystem::Init((GLFWwindow*)m_Window->GetNativeWindow());

		m_ImGui = std::make_unique<VulkanImGui>();
		m_ImGui->Init();
	}

	Application::~Application()
	{
		m_ImGui->Destroy();
		RenderSystem::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(NX_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(NX_BIND_EVENT_FN(Application::OnWindowResize));

		//NX_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime(); // TO BE TRANSFORMED   Platform::GetTime
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			m_Window->ProcessEvent();

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate(timestep);
				}
			}

		
			m_ImGui->BeginFrame();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();

			m_ImGui->EndFrame();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}


	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		//TODO change viewport
		m_Minimized = false;
		return false;
	}


}
