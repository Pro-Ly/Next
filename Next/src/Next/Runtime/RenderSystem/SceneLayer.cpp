#include "nxpch.h"
#include "SceneLayer.h"
#include "Runtime/RenderSystem/RenderSystem.h"


namespace Next {


	SceneLayer::SceneLayer()
	{

	}

	SceneLayer::~SceneLayer()
	{

	}

	void SceneLayer::OnAttach()
	{
		m_SceneRenderer = Ref<SceneRenderer>::Create();
	}

	void SceneLayer::OnDetach()
	{

	}

	void SceneLayer::OnUpdate(Timestep timestep)
	{
		RenderSystem::DrawFrame();
	}

	void SceneLayer::OnImGuiRender()
	{
		/*	ImGui::Begin("Settings");

	for (auto& result : m_ProfileResults)
	{
		char label[50];
		strcpy(label, " %.3fms");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}
	m_ProfileResults.clear();

	ImGui::End();*/
	}

	void SceneLayer::OnEvent(Event& event)
	{

	}

}