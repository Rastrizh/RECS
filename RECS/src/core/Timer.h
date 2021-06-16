#ifndef TIMER_H
#define TIMER_H

#include <chrono>

namespace RECS {

class Timer
{
private:
	static std::chrono::high_resolution_clock::time_point m_StartTime;
	static std::chrono::duration<float> m_deltaTime;

public:
	static auto DeltaTime() ->float;
	static void Reset();
	static void Tick();
};

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

std::chrono::high_resolution_clock::time_point	Timer::m_StartTime = std::chrono::high_resolution_clock::now();
std::chrono::duration<float>					Timer::m_deltaTime = std::chrono::duration<float>(0.F);

}
#endif
