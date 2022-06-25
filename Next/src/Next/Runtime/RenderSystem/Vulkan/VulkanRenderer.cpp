#include "nxpch.h"
#include "VulkanRenderer.h"

namespace Next {


	VulkanRenderer::VulkanRenderer()
	{

	}

	VulkanRenderer::~VulkanRenderer()
	{

	}

	void VulkanRenderer::Init()
	{
		m_Context = Ref<VulkanContext>::Create();

		m_Context->Init();
	}

	void VulkanRenderer::ShutDown()
	{
		m_Context->ShutDown();
	}

	void VulkanRenderer::BeginFrame()
	{
		m_Context->BeginFrame();
	}

	void VulkanRenderer::drawFrame()
	{
		m_Context->DrawFrame();
	}

	void VulkanRenderer::EndFrame()
	{
		m_Context->EndFrame();
	}

}