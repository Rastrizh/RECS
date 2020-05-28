#include "Components.h"
#include "Entities.h"
#include "Engine.h"
#include "EntityContainer.h"
#include <algorithm>

namespace RECS {

EntityContainer::~EntityContainer()
{
	for (auto &p : m_entityContainer)
	{
		delete p.second;
		p.second = nullptr;
	}
}

auto EntityContainer::CreateEntity() ->Entity*
{
	std::lock_guard<std::mutex> Lock(m_entityContainerLocker);
	auto entity = new Entity();
	m_entityContainer[entity->entityID] = entity;
	return entity;
}

void EntityContainer::DeleteEntity(Entity* e)
{
	std::lock_guard<std::mutex> Lock(m_entityContainerLocker);
	m_entityContainer.erase(e->entityID);
	m_ComponentLists.erase(e);
}

auto EntityContainer::GetEntityComponentTypes(Entity * e)->ComponentTypeIDList&
{
	//std::lock_guard<std::mutex> Lock(m_entityContainerLocker);
	return m_ComponentLists[e];
}

auto EntityContainer::GetGroupOfEntities(ComponentTypeIDList&& componentTypeIDs) ->std::vector<Entity*>
{
	std::lock_guard<std::mutex> Lock(m_entityContainerLocker);
	std::vector<Entity*> targets;
	for (auto &e : m_ComponentLists)
	{
		e.second.sort();
		componentTypeIDs.sort();
		ComponentTypeIDList temp_set = Engine::IsIntersect(e.second, componentTypeIDs);
		
		if (temp_set == componentTypeIDs)
		{
			targets.push_back(e.first);
		}
	}
	return targets;
}
}