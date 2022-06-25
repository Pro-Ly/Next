#include "nxpch.h"
#include "RenderSystem.h"
#include "Core/Config.h"

namespace Next {


	RenderSystem::~RenderSystem()
	{

	}

	void RenderSystem::Init(const RendererConfig& rendererConfig)
	{
		s_Config = rendererConfig;

		s_Renderer = RendererAPI::Create(rendererConfig.RendererApi);

		s_Renderer->Init();
	}

	void RenderSystem::Shutdown()
	{
		s_Renderer->ShutDown();
		delete s_Renderer;
		s_Renderer = nullptr;
	}

	void RenderSystem::Tick(Timestep timestep)
	{
		s_CurTimeStep = timestep;
		s_CurTime += timestep;

		s_Renderer->BeginFrame();
		s_Renderer->drawFrame();
		s_Renderer->EndFrame();
	}

}