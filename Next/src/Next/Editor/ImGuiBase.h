#pragma once

namespace Next {

    class ImGuiBase
	{
	public:
		ImGuiBase() = default;
		virtual ~ImGuiBase() = default;

		virtual void Init() = 0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void Destroy() = 0;
		virtual void OnRender() = 0;
	};

}

