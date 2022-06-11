#pragma once
#include "nxpch.h"
#include "Next\Core\Window.h"
#include "Next\Core\Application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "Runtime\RenderSystem\SceneRenderer.h"

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Next {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowSpecification& windowSpec);
		virtual ~WindowsWindow();

		void ProcessEvent() override;

		static inline WindowsWindow& Get() { return (WindowsWindow&)Application::Get().GetWindow(); }
		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		//Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		inline virtual void* GetNativeWindow() const override { return m_Window; }
		virtual void Init() override;
	private:
		virtual void InitWindow(const WindowSpecification& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		WindowSpecification m_WindowSpec;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	
	private:
		void SetGLFWEventCallback();
	};
}