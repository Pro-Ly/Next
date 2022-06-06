#pragma once
#include "nxpch.h"
#include "Next\Window.h"
#include "Next\Platform\Vulkan\VulkanSwapChain.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "Next\Renderer\RendererContext.h"

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Next/Renderer/Pipeline.h"

namespace Next {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowSpecification& windowSpec);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline virtual VulkanSwapChain& GetSwapChain() override { return m_SwapChain; }
		//Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		inline virtual void* GetNativeWindow() const override { return m_Window; }
		inline virtual Ref<RendererContext> GetRenderContext() override { return m_RendererContext; }
		virtual void Init() override;
	private:
		virtual void InitWindow(const WindowSpecification& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		WindowSpecification m_WindowSpec;
		Ref<RendererContext> m_RendererContext;
		VulkanSwapChain m_SwapChain;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;

		Ref<Pipeline> m_Pipeline;//TODO 
	private:
		void SetGLFWEventCallback();
	};
}