#pragma once

#include "nxpch.h"

#include "Next\Core\Ref.h"
#include "Next\Core\Events\Event.h"
namespace Next {

	class VulkanSwapChain;

	struct WindowSpecification
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowSpecification(const std::string& title = "Next Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	//Interface representing a desktop system based Window
	class NEXT_API Window : public RefCounted
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void ProcessEvent() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		//Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowSpecification& spec = WindowSpecification());
		virtual void Init() = 0;
		virtual void Shutdown() = 0;
	};
}

