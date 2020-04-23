#define _CRTDBG_MAP_ALLOC

#include <crtdbg.h>;
#include "Components.h"
#include <iostream>

using namespace RECS;

class MyComponent : public Component<MyComponent>
{
public:
	MyComponent(int _x, int _y)
		:
		x(_x),
		y(_y)
	{}
	int x = 0;
	int y = 1;
};

class MyEntity : public IEntity
{
};

int main()
{
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);

	IEntity e;
	
	e.AddComponent<MyComponent>(2, 3);

	for (const auto &p : ComponentContainer::container[MyComponent::GetTypeID()])
	{
		std::cout << p.first << ' ' << (dynamic_cast<MyComponent*>(p.second))->y << std::endl;
	}
	
	auto *p = e.GetComponent<MyComponent>();
	std::cout << p->x;
	e.~IEntity();
	_CrtDumpMemoryLeaks();
	return 0;
}