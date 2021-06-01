#ifndef TIMER_H
#define TIMER_H

#include <chrono>

namespace RECS {

class Timer
{
private:
	static Timer* s_instance;
	std::chrono::high_resolution_clock::time_point m_StartTime;
	std::chrono::duration<float> m_deltaTime;

public:
	static auto instance() ->Timer*;
	auto DeltaTime() ->float;
	void Reset();
	void Tick();
	
private:
	Timer();
};
}
#endif
