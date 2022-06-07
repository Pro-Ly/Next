#pragma once

#include "RendererContext.h"
#include "Next/Application.h"
#include "RendererAPI.h"
#include "RenderCommandBuffer.h"
#include "RenderCommandQueue.h"

namespace Next {

	static RendererAPI* s_RendererAPI = nullptr;
	static RenderCommandQueue* s_CommandQueue = nullptr;

	class Renderer 
	{
	public:
		inline static Ref<RendererContext> GetContext() { return Application::Get().GetWindow().GetRenderContext(); }
		static RendererAPI* InitRendererAPI();
		static void Init();
		static void Shutdown();
		static void BeginRenderPass(const Ref<RenderCommandBuffer>& commandBuffer);
		template<typename FuncT>
		static void Submit(FuncT&& func)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();

				// NOTE: Instead of destroying we could try and enforce all items to be trivally destructible
				// however some items like uniforms which contain std::strings still exist for now
				// static_assert(std::is_trivially_destructible_v<FuncT>, "FuncT must be trivially destructible");
				pFunc->~FuncT();
			};
			auto storageBuffer = GetRenderCommandQueue().Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
	private:
		static RenderCommandQueue& GetRenderCommandQueue();
	};
}


