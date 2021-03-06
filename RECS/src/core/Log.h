#ifndef LOG_H
#define LOG_H

#include "../RECSTypes.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"

class Log
{
public:
	static void Init();

	inline static RECS::ref<spdlog::logger>& GetLogger() { return s_logger; }

private:
	static RECS::ref<spdlog::logger> s_logger;
};

void Log::Init()
{
	s_logger->set_level(spdlog::level::debug);
}

RECS::ref<spdlog::logger> Log::s_logger = spdlog::stdout_color_mt("s_logger");

#define RTRACE(...) Log::GetLogger()->trace(__VA_ARGS__)
#define RINFO(...)  Log::GetLogger()->info(__VA_ARGS__)
#define RWARN(...)  Log::GetLogger()->warn(__VA_ARGS__)
#define RERROR(...) Log::GetLogger()->error(__VA_ARGS__)
#define RFATAL(...) Log::GetLogger()->fatal(__VA_ARGS__)

#endif // !LOG_H
