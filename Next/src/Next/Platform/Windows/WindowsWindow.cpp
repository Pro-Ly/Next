#include "nxpch.h"
#include "WindowsWindow.h"

#include "Next\Events\ApplicationEvent.h"
#include "Next\Events\KeyEvent.h"
#include "Next\Events\MouseEvent.h"

#include "Next/Platform/Windows/WindowsWindow.h"
#include "Next/Platform/Windows/WindowsInput.h"

#include "Next/Renderer/RendererAPI.h"
#include "Next/Renderer/Pipeline.h"

#include "Next/Platform/Vulkan/VulkanContext.h"

namespace Next {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		NX_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowSpecification& windowSpec)
	{
		return new WindowsWindow(windowSpec);
	}

	WindowsWindow::WindowsWindow(const WindowSpecification& windowSpec)
		:m_WindowSpec(windowSpec)
	{
	
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::InitWindow(const WindowSpecification& windowSpec)
	{
		m_Data.Title = windowSpec.Title;
		m_Data.Width = windowSpec.Width;
		m_Data.Height = windowSpec.Height;

		NX_CORE_INFO("Creating window {0} ({1}, {2})", windowSpec.Title, windowSpec.Width, windowSpec.Height);

		if (!s_GLFWInitialized)
		{
			// TODO : glfwTerminate on system shutdown
			int success = glfwInit();
			NX_CORE_ASSERT(success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		//glfw was originally designed to create an opengl context , so we need to tell it to not create an opengl context
		if (RendererAPI::GetCurrentType() == RendererAPIType::Vulkan)
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // TEMP NOT RESIZE

		m_Window = glfwCreateWindow((int)windowSpec.Width, (int)windowSpec.Height, m_Data.Title.c_str(), nullptr, nullptr);

		//Create Renderer Context
		m_RendererContext = RendererContext::Create(m_Window);
		m_RendererContext->Init();

		Ref<VulkanContext> vkContext = m_RendererContext.As<VulkanContext>();
		m_SwapChain.Init(vkContext->GetVKInstance(), vkContext->GetDevice());
		m_SwapChain.Create(m_Window);

		PipelineSpecification spec;
		m_Pipeline = Pipeline::Create(spec);

		glfwSetWindowUserPointer(m_Window, &m_Data);

		SetGLFWEventCallback();
	}


	void WindowsWindow::Shutdown()
	{
		m_SwapChain.Destroy();
		glfwTerminate();
	}

	void WindowsWindow::SetGLFWEventCallback()
	{
		//Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				key = WindowsInput::NativeKeyCodeToNXKeyCode(key);

				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, 1);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int character)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(character);
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				button = WindowsInput::NativeKeyCodeToNXKeyCode(button);

				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
	}

	void WindowsWindow::Init()
	{
		InitWindow(m_WindowSpec);
		m_SceneRenderer = Ref<SceneRenderer>::Create();
	}

}