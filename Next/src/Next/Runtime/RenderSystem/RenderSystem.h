#pragma once

#include "Next/Core\Application.h"
#include "Core/Config.h"
#include "RendererAPI.h"

#include <memory>

namespace Next {

	class RenderSystem
	{
	public:
		RenderSystem() = default;
		virtual ~RenderSystem();
		inline static const RendererConfig& GetConfig() { return s_Config; };
		inline static RendererAPI* GetRenderer() { return s_Renderer; };
		static void Init(const RendererConfig& rendererConfig);
		static void Shutdown();
		static void Tick(Timestep timestep);

		inline static const double GetCurTime() { return s_CurTime; }
		inline static const Timestep GetCurTimeStep() { return s_CurTimeStep; }

	private:
		inline static RendererAPI* s_Renderer = nullptr;
		inline static RendererConfig s_Config;

	private:
		inline static double s_CurTime = 0.0;
		inline static Timestep s_CurTimeStep = 0.0;
	};
}