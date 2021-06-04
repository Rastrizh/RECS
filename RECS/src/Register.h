#ifndef REGISTER_H
#define REGISTER_H

#include <map>
#include <set>
#include <vector>
#include <array>
#include <mutex>

#include "core/Log.h"
#include "RECSTypes.h"
#include "Events/Event.h"

#include "memory/MemoryManager.h"
#include "EntityManager.h"
#include "ComponentManager.h"

namespace RECS {

class Entity;

class Engine
{
public:
	static std::mutex s_Engine_lock;
	static std::map<entityID, std::map<ComponentTypeID, IComponent*>> s_entity_components;
	static std::map<ComponentTypeID, std::set<entityID>> s_entity_table;

	static event<entityID> OnEntityCreated;
	static event<entityID> OnEntityDestroyed;

public:
	static void Initialize()
	{
		Engine::OnEntityCreated += [](entityID eid) {

		};
		Engine::OnEntityDestroyed += [](entityID eid) {
			ComponentManager::DeleteEntity(s_entity_components[eid]);
			EntityManager::DeleteEntity(eid);
		};

		ComponentManager::OnComponentAdded += [](entityID eid, ComponentTypeID cid, IComponent* component) {
			Engine::ComponentAdded(eid, cid, component);
		};
		ComponentManager::OnComponentRemoved += [](entityID eid, ComponentTypeID cid) {
			Engine::ComponentRemoved(eid, cid);
		};
	}
	static Entity* CreateEntity()
	{
		return EntityManager::CreateEntity();
	}

	static void KillEntity(entityID eid)
	{
		std::lock_guard<std::mutex> Lock(s_Engine_lock);
		OnEntityDestroyed(eid);
		s_entity_components.erase(eid);
		for (auto& s : s_entity_table)
			s.second.erase(eid);
	}
	static void KillAllEntities()
	{
		for (auto &e : s_entity_components)
		{
			KillEntity(e.first);
		}
	}

	template<class ... Args>
	static std::set<entityID> getGroup()
	{
		std::lock_guard<std::mutex> Lock(s_Engine_lock);
		std::vector<ComponentTypeID> v{ (Args::GetTypeID())... };
		auto temp = s_entity_table[v[0]];
		if (v.size() < 2)
			return temp;
		std::set<entityID> target;
		for (size_t i = 1; i < v.size(); i++)
		{
			target = s_entity_table[v[i]];
			temp.merge(target);
			temp = target;
		}
		return target;
	}

	template<class T>
	static T* getComponent(const entityID& eid)
	{
		std::lock_guard<std::mutex> Lock(s_Engine_lock);
		return (T*)s_entity_components[eid][T::GetTypeID()];
	}
	template<class T>
	static bool hasComponent(const entityID& eid)
	{
		std::lock_guard<std::mutex> Lock(s_Engine_lock);
		return !s_entity_components[eid].empty();
	}
	static void ComponentAdded(const entityID& eid, const ComponentTypeID& componentType, IComponent* component)
	{
		std::lock_guard<std::mutex> Lock(s_Engine_lock);
		s_entity_components[eid][componentType] = component;
		s_entity_table[componentType].insert(eid);
	}
	static void ComponentRemoved(const entityID& eid, const ComponentTypeID& componentType)
	{
		std::lock_guard<std::mutex> Lock(s_Engine_lock);
		s_entity_table.erase(componentType);
		s_entity_components[eid].erase(componentType);
	}
};

std::mutex Engine::s_Engine_lock;

std::map<entityID, std::map<ComponentTypeID, IComponent*>> Engine::s_entity_components;

std::map<ComponentTypeID, std::set<entityID>> Engine::s_entity_table;

event<entityID> Engine::OnEntityCreated;
event<entityID> Engine::OnEntityDestroyed;

}
#endif // !REGISTER_H
