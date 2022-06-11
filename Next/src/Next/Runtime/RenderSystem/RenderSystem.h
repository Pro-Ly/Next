#pragma once

#include "Next/Core\Application.h"
#include "RendererAPI.h"

#include <memory>

extern struct GLFWwindow;

namespace Next {

	static RendererAPI* s_RendererAPI = nullptr;

	class RenderSystem
	{
	public:
		RenderSystem() = default;
		virtual ~RenderSystem();
		static void Select(RendererAPIType apiType);
		static void Init(GLFWwindow* window);
		static void Shutdown();
		static void DrawFrame();
		//template<typename FuncT>
		//static void Submit(FuncT&& func)
		//{
		//	auto renderCmd = [](void* ptr) {
		//		auto pFunc = (FuncT*)ptr;
		//		(*pFunc)();

		//		// NOTE: Instead of destroying we could try and enforce all items to be trivally destructible
		//		// however some items like uniforms which contain std::strings still exist for now
		//		// static_assert(std::is_trivially_destructible_v<FuncT>, "FuncT must be trivially destructible");
		//		pFunc->~FuncT();
		//	};
		//	auto storageBuffer = GetRenderCommandQueue().Allocate(renderCmd, sizeof(func));
		//	new (storageBuffer) FuncT(std::forward<FuncT>(func));
		//}
	private:
		static GLFWwindow* m_Window;
	};
}