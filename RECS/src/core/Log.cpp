#include "Log.h"

void Log::Init()
{
	s_logger->set_level(spdlog::level::debug);
}


RECS::ref<spdlog::logger> Log::s_logger = spdlog::stdout_color_mt("s_logger");
