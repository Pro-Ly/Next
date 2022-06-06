#pragma once

#include "Core/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Next {

	class NEXT_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define NX_CORE_TRACE(...)     ::Next::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define NX_CORE_INFO(...)      ::Next::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NX_CORE_WARN(...)      ::Next::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NX_CORE_ERROR(...)     ::Next::Log::GetCoreLogger()->error(__VA_ARGS__)
#define NX_CORE_CRITICAL(...)  ::Next::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define NX_TRACE(...)	       ::Next::Log::GetClientLogger()->trace(__VA_ARGS__)
#define NX_INFO(...)	       ::Next::Log::GetClientLogger()->info(__VA_ARGS__)
#define NX_WARN(...)	       ::Next::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NX_ERROR(...)	       ::Next::Log::GetClientLogger()->error(__VA_ARGS__)
#define NX_CRITICAL(...)       ::Next::Log::GetClientLogger()->critical(__VA_ARGS__)

