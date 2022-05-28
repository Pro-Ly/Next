#include "hzpch.h"
#include "OpenGLContext.h"

#include <GLFW\glfw3.h>
#include <glad\glad.h>

namespace Next {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		NX_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		NX_CORE_ASSERT(status, "Failed to initialize Glad!");

		NX_CORE_INFO("OpenGL Info:");
		NX_CORE_INFO("  Vendor :{0}",glGetString(GL_VENDOR));
		NX_CORE_INFO("  Renderer :{0}",glGetString(GL_RENDERER));
		NX_CORE_INFO("  Version :{0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}

