#pragma once

#include "Next/Core/Layer.h"

#include "Next/Runtime/RenderSystem/Scene.h"

namespace Next {

	template<typename Fn>
	class Timer
	{
	public:
		Timer(const char* name, Fn&& func)
			:m_Name(name), m_Func(func), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~Timer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			m_Stopped = true;

			float duration = (end - start) * 0.001f;

			std::cout << "Duration: " << duration << "ms" << std::endl;
			m_Func({ m_Name,duration });
		}

	private:
		const char* m_Name;
		Fn m_Func;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};

	#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); });


	class EngineLayer : public Layer
	{
	public:
		EngineLayer();
		virtual ~EngineLayer() override;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep timestep) override;
		virtual void OnEvent(Event& event) override;
	public:
		Ref<Scene> m_Scene;
		Ref<SceneRenderer> m_SceneRenderer;

		struct ProfileResult
		{
			const char* Name;
			float Time;
		};

		std::vector<ProfileResult> m_ProfileResults;
	};


}

