#include "Entities.h"
#include "Engine.h"
#include "EntityContainer.h"
#include <algorithm>

namespace RECS {
auto Engine::instance() -> Engine*
{
	static auto instance = new Engine();
	return instance;
}

Engine::Engine()
{
	m_EntityContainer = std::make_unique<EntityContainer>();
	OnEntityDestroyed += [&](Entity* e) {

		m_EntityContainer->DeleteEntity(e);
	};
}

auto Engine::CreateEntity() -> Entity *
{
	auto e = m_EntityContainer->CreateEntity();
	
	e->OnComponentAdded += [this](Entity* e, ComponentType componentType)
	{
		ComponentAdded(e, componentType);
	};

	e->OnComponentRemoved += [this](Entity* e, ComponentType componentType)
	{
		ComponentRemoved(e, componentType);
	};
	
	OnEntityCreated(e);
	return e;
}
void Engine::KillEntity(Entity* e)
{
	std::list<ComponentType> entityComponents = m_EntityContainer->GetEntityComponentTypes(e);
	for (auto g : m_groups)
	{
		std::list<ComponentType> temp_set = IsIntersect(entityComponents, g.second->GetSignature());

		if (temp_set == g.second->GetSignature())
		{
			m_groups[temp_set]->OnEntityDeleted(e);
		}
	}
	OnEntityDestroyed(e);
}
void Engine::KillAllEntities()
{
}
auto Engine::GetGroup(std::list<ComponentType>&& componentTypeIDs) -> Group*
{
	if (m_groups.find(componentTypeIDs) == m_groups.end())
	{
		auto group = new Group(std::move(componentTypeIDs));
		group->OnEntityChanged += [group](Entity* e) {
			group->AddOrRemoveChangedEntity(e);
		};
		group->OnEntityDeleted += [group](Entity* e) {
			
			group->RemoveEntity(e);
		};

		m_groups[componentTypeIDs] = group;
		return group;
	}
	return m_groups[componentTypeIDs];
}

auto Engine::GetGroupOfEntities(std::list<ComponentType>&& componentTypeIDs) ->std::vector<Entity*>
{
	return m_EntityContainer->GetGroupOfEntities(std::move(componentTypeIDs));
}

auto Engine::GetEntityComponentTypes(Entity * e)->std::list<ComponentType>&
{
	return m_EntityContainer->GetEntityComponentTypes(e);
}

void Engine::ComponentAdded(Entity * e, ComponentType componentType)
{
	m_EntityContainer->m_ComponentLists[e].push_back(componentType);
	for (auto g : m_groups)
	{
		g.second->OnEntityChanged(e);
	}
}

void Engine::ComponentRemoved(Entity * e, ComponentType componentType)
{
	m_EntityContainer->m_ComponentLists[e].remove(componentType);
	for (auto g : m_groups)
	{
		g.second->OnEntityChanged(e);
	}
}

auto Engine::IsIntersect(std::list<ComponentType>& inWhat, std::list<ComponentType>& What)->std::list<RECS::ComponentType>
{
	std::list<ComponentType> ret;
	inWhat.sort();
	What.sort();
	std::set_intersection(inWhat.begin(), inWhat.end(),	What.begin(), What.end(), std::back_inserter(ret));
	return ret;
}
}