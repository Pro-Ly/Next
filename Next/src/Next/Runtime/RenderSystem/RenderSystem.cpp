#include "nxpch.h"
#include "RenderSystem.h"
#include "Runtime/RenderSystem/RendererAPI.h"

namespace Next {

	GLFWwindow* RenderSystem::m_Window = nullptr;

	RenderSystem::~RenderSystem()
	{

	}

	void RenderSystem::Select(RendererAPIType apiType)
	{
		s_RendererAPI = RendererAPI::Create(apiType);
	}

	void RenderSystem::Init(GLFWwindow* window)
	{
		m_Window = window;
		s_RendererAPI->Init(window);
	}

	void RenderSystem::Shutdown()
	{
		if (s_RendererAPI)
		{
			s_RendererAPI->Shutdown();
			s_RendererAPI = nullptr;
		}
	}

	void RenderSystem::DrawFrame()
	{
		s_RendererAPI->drawFrame();
	}

}