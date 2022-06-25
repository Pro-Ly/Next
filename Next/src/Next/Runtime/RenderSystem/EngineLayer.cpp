#include "nxpch.h"
#include "EngineLayer.h"
#include "Runtime/RenderSystem/RenderSystem.h"


namespace Next {


	EngineLayer::EngineLayer()
	{

	}

	EngineLayer::~EngineLayer()
	{

	}

	void EngineLayer::OnAttach()
	{
		m_SceneRenderer = Ref<SceneRenderer>::Create();
	}

	void EngineLayer::OnDetach()
	{
		
	}

	void EngineLayer::OnUpdate(Timestep timestep)
	{
		PROFILE_SCOPE("1")
	}

	void EngineLayer::OnEvent(Event& event)
	{

	}

}