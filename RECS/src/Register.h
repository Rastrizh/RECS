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
#include "View.h"

namespace RECS {

class Entity;

class Engine
{
public:
	static std::mutex s_Engine_lock;
	//static std::map<entityID, std::map<ComponentTypeID, IComponent*>> s_entity_components;
	//static std::map<ComponentTypeID, std::set<entityID>> s_entity_table;

	static event<Entity*> OnEntityCreated;
	static event<Entity*> OnEntityDestroyed;

public:
	static void Initialize()
	{
		//Engine::OnEntityDestroyed += Engine::DeleteEntity;
		//Engine::OnEntityDestroyed += EntityManager::DeleteEntity;

		ComponentManager::OnComponentAdded += Engine::ComponentAdded;

		ComponentManager::OnComponentRemoved += Engine::ComponentRemoved;
	}

	static Entity* CreateEntity()
	{
		return EntityManager::CreateEntity();
	}

	static void KillEntity(Entity* e)
	{
		//std::lock_guard<std::mutex> Lock(s_Engine_lock);

		e->isUpdateble = false;
		ComponentManager::DeleteEntityComponents(e);
		EntityManager::DeleteEntity(e);
		OnEntityDestroyed(e);
	}
	static void KillAllEntities()
	{
		EntityManager::DeleteAll();
		ComponentManager::Clear();
	}

	//static void DeleteEntity(const entityID& eid)
	//{
	//	ComponentManager::DeleteEntity(s_entity_components[eid]);
	//}

	template<typename ...Types>
	static void each(std::common_type_t<std::function<void(Entity*, ComponentHandle<Types>...)>> view)
	{
		auto count = EntityManager::TotalEntities();
		for (size_t i = 0; i < count; i++)
		{
			auto e = EntityManager::GetEntity(i);
			if(e && e->isUpdateble && e->HasComponent<Types...>())
				view(e, e->GetComponent<Types>()...);
			else
				continue;
		}
	}

	//template<class ... Args>
	//static std::set<entityID> getGroup()
	//{
	//	//std::lock_guard<std::mutex> Lock(s_Engine_lock);
	//	std::vector<ComponentTypeID> v{ (Args::GetTypeID())... };
	//	auto temp = s_entity_table[v[0]];
	//	if (v.size() < 2)
	//		return temp;
	//	std::set<entityID> target;
	//	for (size_t i = 1; i < v.size(); i++)
	//	{
	//		target = s_entity_table[v[i]];
	//		temp.merge(target);
	//		temp = target;
	//	}
	//	return target;
	//}

	//template<class T>
	//static T* getComponent(const entityID& eid)
	//{
	//	//std::lock_guard<std::mutex> Lock(s_Engine_lock);
	//	return (T*)s_entity_components[eid][T::GetTypeID()];
	//}
	//template<class T>
	//static bool hasComponent(const entityID& eid)
	//{
	//	std::lock_guard<std::mutex> Lock(s_Engine_lock);
	//	return !s_entity_components[eid].empty();
	//}
	static void ComponentAdded(const entityID& eid, const ComponentTypeID& componentType, IComponent* component)
	{
		std::lock_guard<std::mutex> Lock(s_Engine_lock);
		//s_entity_components[eid][componentType] = component;
		//s_entity_table[componentType].insert(eid);
	}
	static void ComponentRemoved(const entityID& eid, const ComponentTypeID& componentType)
	{
		std::lock_guard<std::mutex> Lock(s_Engine_lock);
		//s_entity_table.erase(componentType);
		//s_entity_components[eid].erase(componentType);
	}
};

std::mutex Engine::s_Engine_lock;

//std::map<entityID, std::map<ComponentTypeID, IComponent*>> Engine::s_entity_components;

//std::map<ComponentTypeID, std::set<entityID>> Engine::s_entity_table;

event<Entity*> Engine::OnEntityCreated;
event<Entity*> Engine::OnEntityDestroyed;

}
#endif // !REGISTER_H
