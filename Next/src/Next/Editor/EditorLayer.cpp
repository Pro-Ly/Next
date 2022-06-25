#include "nxpch.h"
#include "EditorLayer.h"
#include "VulkanImGui.h"
#include "Next/Core/KeyCodes.h"
#include "imgui.h"
#include "Next/Core/Application.h"
#include "Runtime/RenderSystem/EngineLayer.h"

namespace Next {

	EditorLayer::EditorLayer(EngineLayer& scenelayer)
		:Layer("EditorLayer"), m_SceneLayer(scenelayer)
	{

	}
	EditorLayer::~EditorLayer()
	{
	}

	void EditorLayer::OnAttach()
	{
		m_ImGui = std::make_unique<VulkanImGui>(this);
		m_ImGui->Init();
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnImGuiRender()
	{

	}

	void EditorLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& event) { return OnKeyPressedEvent(event); });
		dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& event) { return OnMouseButtonPressed(event); });
		dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& event) { return OnWindowCloseEvent(event); });
	}

	void EditorLayer::OnUpdate(Next::Timestep timestep)
	{

	}

	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
			case NX_KEY_S:
				NX_CORE_INFO("S");
				break;
		}
		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		return false;
	}

	bool EditorLayer::OnWindowCloseEvent(WindowCloseEvent& e)
	{
		return false;
	}

}