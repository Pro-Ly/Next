#pragma once
#include "nxpch.h"
#include "Next\Window.h"
#include "Next\Renderer\RendererContext.h"
#include "Next\Platform\Vulkan\VulkanSwapChain.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Next {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		//Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		inline virtual void* GetNativeWindow() const override { return m_Window; }
	private:
		virtual void InitWindow(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
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
	};
}