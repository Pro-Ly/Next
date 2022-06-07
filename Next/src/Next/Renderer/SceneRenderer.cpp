#include "nxpch.h"
#include "SceneRenderer.h"
#include "Renderer.h"

namespace Next {


	SceneRenderer::SceneRenderer()
	{
		m_CommandBuffer = RenderCommandBuffer::Create();
		Renderer::BeginRenderPass(m_CommandBuffer);
	}

	SceneRenderer::~SceneRenderer()
	{

	}

}