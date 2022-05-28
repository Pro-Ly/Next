#include "hzpch.h"
#include "WindowsInput.h"

#include "Hazel\Application.h"
#include <GLFW\glfw3.h>

namespace Hazel {

	Input* Input::s_Instance = new WindowsInput();

	int WindowsInput::HZKeyCodeToNativeKeyCodeImpl(int keycode)
	{
		return keycode;
	}

	int WindowsInput::NativeKeyCodeToHZKeyCodeImpl(int keycode)
	{
		return keycode;
	}

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		keycode = WindowsInput::HZKeyCodeToNativeKeyCode(keycode);
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		button = WindowsInput::HZKeyCodeToNativeKeyCode(button);
		auto state = glfwGetKey(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos,(float)ypos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}
}
