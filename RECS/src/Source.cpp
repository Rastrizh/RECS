#define _CRTDBG_MAP_ALLOC

#include <crtdbg.h>
#include <iostream>
#include <chrono>
#include "Components.h"
#include "Systems.h"
#include "EntityContainer.h"
#include "ComponentContainer.h"
#include "Engine.h"

using namespace RECS;

class Move : public Component<Move>
{
public:
	Move(float _x, float _y)
		:
		x(_x),
		y(_y)
	{}
	float x = 0.001F;
	float y = 0.001F;
};

class Position : public Component<Position>
{
public:
	Position(float _x, float _y)
		:
		x(_x),
		y(_y)
	{}
	float x = 0.001F;
	float y = 0.001F;
};

class MoveSystem : public ISystem
{
public:
	~MoveSystem() final = default;
	void CreatePool() final
	{
		m_targetGroup = Engine::instance().GetGroup(std::move(CREATE_TARGET_ENTITY_ID_LIST(Move, Position)));
	}

	void Update() final
	{
		for (auto e : m_targetGroup->GetEntities())
		{
			auto move = e->GetComponent<Move>();
			auto pos = e->GetComponent<Position>();
			pos->x += move->x;
			pos->y += move->y;
		}
	}
};

auto main() -> int
{
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);

	Engine &engine = Engine::instance();

	std::vector<Entity*> ve;
	auto begin = std::chrono::steady_clock::now();
	for (size_t i = 0; i < 150000; i++)
	{
		ve.push_back(engine.CreateEntity());
	}
	auto end = std::chrono::steady_clock::now();
	auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	std::cout << "The time: " << elapsed_ms.count() << " ms\n";

	auto begin2 = std::chrono::steady_clock::now();
	for (auto &e : ve)
	{
		e->AddComponent<Move>(0.002F, 0.002F);
		e->AddComponent<Position>(0.002F, 0.002F);
	}
	auto end2 = std::chrono::steady_clock::now();
	auto elapsed_ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - begin2);
	std::cout << "The time: " << elapsed_ms2.count() << " ms\n";
	

	/*Entity* e = engine.CreateEntity();
	Entity* e2 = engine.CreateEntity();
	Entity* e3 = engine.CreateEntity();

	//e->AddComponent<Move>(0.001F, 0.001F);
	e->AddComponent<Position>(0.001F, 0.001F);

	e2->AddComponent<Move>(0.002F, 0.002F);
	e2->AddComponent<Position>(0.002F, 0.002F);

	e3->AddComponent<Move>(0.003F, 0.003F);
	e3->AddComponent<Position>(0.003F, 0.003F);
	*/
	
	MoveSystem sys;
	auto begin3 = std::chrono::steady_clock::now();
	sys.CreatePool();
	auto end3 = std::chrono::steady_clock::now();
	auto elapsed_ms3 = std::chrono::duration_cast<std::chrono::milliseconds>(end3 - begin3);
	std::cout << "The time: " << elapsed_ms3.count() << " ms\n";
	

	while (true)
	{
		auto begin4 = std::chrono::steady_clock::now();
		sys.Update();
		auto end4 = std::chrono::steady_clock::now();
		auto elapsed_ms4 = std::chrono::duration_cast<std::chrono::milliseconds>(end4 - begin4);
		std::cout << "The time: " << elapsed_ms4.count() << " ms\n";

		//system("CLS");
	}
	
	/*while (true)
	{
		sys.Update();
		
		std::cout << e->GetComponent<Position>()->x << ' ' << e->GetComponent<Position>()->y << "\n" << std::flush;
		std::cout << e2->GetComponent<Position>()->x << ' ' << e2->GetComponent<Position>()->y << "\n" << std::flush;
		std::cout << e3->GetComponent<Position>()->x << ' ' << e3->GetComponent<Position>()->y << "\n" << std::flush;
		system("CLS");
	}
	*/

	/*for (const auto &p : ComponentContainer::container)
	{
		std::cout << p.first << "\n";
	}
		
	for (const auto &p : ComponentContainer::container[COMPONENT_GET_TYPE_ID(A)])
	{
		std::cout << p.first << ' ' << (dynamic_cast<A*>(p.second))->x << ' ' << (dynamic_cast<A*>(p.second))->y << "\n";
	}

	for (const auto &p : ComponentContainer::container[COMPONENT_GET_TYPE_ID(B)])
	{
		std::cout << p.first << ' ' << (dynamic_cast<B*>(p.second))->x << ' ' << (dynamic_cast<B*>(p.second))->y << "\n";
	}

	auto *p = e.GetComponent<A>();
	std::cout << p->x << "\n";*/
	//entityContainer.~EntityContainer();
	//ComponentContainer::instance().~ComponentContainer();
	//sys.~MoveSystem();
	_CrtDumpMemoryLeaks();
	std::cin.get();
	return 0;
}