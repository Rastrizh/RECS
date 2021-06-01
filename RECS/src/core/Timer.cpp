#include "Timer.h"

namespace RECS {

Timer* Timer::s_instance = nullptr;

auto Timer::DeltaTime() ->float
{
	return m_deltaTime.count();
}

void Timer::Reset()
{
	m_StartTime = std::chrono::high_resolution_clock::now();
}

void Timer::Tick()
{
	m_deltaTime = std::chrono::high_resolution_clock::now() - m_StartTime;
}

auto Timer::instance() -> Timer *
{
	s_instance = new Timer();
	return s_instance;
}

Timer::Timer()
{
	m_StartTime = std::chrono::high_resolution_clock::now();
	m_deltaTime = std::chrono::duration<float>(0.F);
}
}