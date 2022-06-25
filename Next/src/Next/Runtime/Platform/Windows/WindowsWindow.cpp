#include "nxpch.h"
#include "WindowsWindow.h"

#include "Core\Events\ApplicationEvent.h"
#include "Core\Events\KeyEvent.h"
#include "Core\Events\MouseEvent.h"

#include "Runtime/Platform/Windows/WindowsWindow.h"
#include "Runtime/Platform/Windows/WindowsInput.h"

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
			NX_CORE_ASSERT(glfwInit(), "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);//Only for Vulkan!!!

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // TODO TEMP NOT RESIZE

		m_Window = glfwCreateWindow((int)windowSpec.Width, (int)windowSpec.Height, m_Data.Title.c_str(), nullptr, nullptr);

		// Update window size to actual size
		{
			int width, height;
			glfwGetWindowSize(m_Window, &width, &height);
			m_Data.Width = width;
			m_Data.Height = height;
		}

		glfwSetWindowUserPointer(m_Window, &m_Data);

		SetGLFWEventCallback();
	}


	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
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

	void WindowsWindow::ProcessEvent()
	{
		glfwPollEvents();
	}

	void WindowsWindow::Init()
	{
		InitWindow(m_WindowSpec);

		if (glfwRawMouseMotionSupported())
		{
			glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}
		else
		{
			NX_CORE_ERROR("GLFW don't support raw motion input");
		}
	}

}