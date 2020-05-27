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

EntityContainer::EntityContainer()
{
	m_EngineInstance = Engine::instance();
}

auto EntityContainer::CreateEntity() ->Entity*
{
	auto entity = new Entity();
	m_entityContainer[entity->entityID] = entity;
	return entity;
}

void EntityContainer::DeleteEntity(Entity* e)
{
	m_entityContainer.erase(e->entityID);
	m_ComponentLists.erase(e);
}

auto EntityContainer::GetEntityComponentTypes(Entity * e)->ComponentTypeIDList&
{
	return m_ComponentLists[e];
}

auto EntityContainer::GetGroupOfEntities(ComponentTypeIDList&& componentTypeIDs) ->std::vector<Entity*>
{
	std::vector<Entity*> targets;
	for (auto &e : m_ComponentLists)
	{
		ComponentTypeIDList temp_set = m_EngineInstance->IsIntersect(e.second, componentTypeIDs);
		
		if (temp_set == componentTypeIDs)
		{
			targets.push_back(e.first);
		}
	}
	return targets;
}
}