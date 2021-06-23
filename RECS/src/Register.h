#ifndef REGISTER_H
#define REGISTER_H

#include <map>
#include <set>
#include <vector>
#include <array>
#include <mutex>
#include <memory>
#include <functional>

#include "core/Log.h"
#include "RECSTypes.h"
#include "Events/Event.h"

#include "memory/MemoryManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"

namespace RECS {

class Entity;

class Engine
{
public:
	static std::mutex s_Engine_lock;

	static event<Entity*> OnEntityCreated;
	static event<Entity*> OnEntityDestroyed;

public:

	static Entity*	CreateEntity();
	static void			KillEntity(Entity* e);
	static void			KillAllEntities();

	static size_t		EntityCount();

	template<typename ...Types>
	static void each(std::common_type_t<std::function<void(Entity*, ComponentHandle<Types>...)>> view)
	{
		auto count = EntityManager::TotalEntities();
		for (entityID i = 0; i < count; i++)
		{
			auto e = EntityManager::GetEntity(i);
			if(e && e->isUpdateble && e->HasComponent<Types...>())
				view(e, e->GetComponent<Types>()...);
			else
				continue;
		}
	}
	template<typename ...Types>
	static std::vector<Entity*> getView()
	{
		std::vector<Entity*> view;
		auto count = EntityManager::TotalEntities();
		for (entityID i = 0; i < count; i++)
		{
			auto e = EntityManager::GetEntity(i);
			if (e && e->isUpdateble && e->HasComponent<Types...>())
				view.push_back(e);
		}
		return view;
	}
};

Entity* Engine::CreateEntity()
{
	return EntityManager::CreateEntity();
}

size_t Engine::EntityCount()
{
	return EntityManager::EntityCount();
}

void Engine::KillEntity(Entity* e)
{
	ComponentManager::DeleteEntityComponents(e);
	EntityManager::DeleteEntity(e);
	OnEntityDestroyed(e);
}

void Engine::KillAllEntities()
{
	EntityManager::DeleteAll();
	ComponentManager::Clear();
}

std::mutex Engine::s_Engine_lock;

event<Entity*> Engine::OnEntityCreated;
event<Entity*> Engine::OnEntityDestroyed;

}
#endif // !REGISTER_H
