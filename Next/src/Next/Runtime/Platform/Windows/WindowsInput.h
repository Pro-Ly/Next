#pragma once

#include "Next\Core\Input.h"

namespace Next {

	class WindowsInput : public Input
	{
	protected:
		virtual int NXKeyCodeToNativeKeyCodeImpl(int keycode) override;
		virtual int NativeKeyCodeToNXKeyCodeImpl(int keycode) override;
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}


