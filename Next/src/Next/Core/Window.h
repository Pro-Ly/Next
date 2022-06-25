#pragma once

#include "nxpch.h"

#include "Next\Core\Ref.h"
#include "Next\Core\Events\Event.h"
#include "Config.h"

namespace Next {

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


