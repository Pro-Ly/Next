#pragma once

#include "nxpch.h"

#include "Next\Core\Ref.h"
#include "Next\Events\Event.h"
#include "Next\Renderer\RendererContext.h"

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

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual VulkanSwapChain& GetSwapChain() = 0;
		//Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowSpecification& spec = WindowSpecification());
		virtual void Init() = 0;

		virtual Ref<RendererContext> GetRenderContext() = 0;
	};
}


