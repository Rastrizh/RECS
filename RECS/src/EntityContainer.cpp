#include "Components.h"
#include "Entities.h"
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
	auto entity = new Entity();
	m_entityContainer[entity->entityID] = entity;
	return entity;
}

auto EntityContainer::GetGroupOfEntities(std::list<ComponentType>&& componentTypeIDs) ->std::vector<Entity*>
{
	std::vector<Entity*> targets;
	for (auto &e : m_ComponentLists)
	{
		std::list<ComponentType> temp_set;
		e.second.sort();
		componentTypeIDs.sort();
		std::set_intersection(e.second.begin(), e.second.end(), componentTypeIDs.begin(), componentTypeIDs.end(), std::back_inserter(temp_set));
		if (temp_set == componentTypeIDs)
		{
			targets.push_back(e.first);
		}
	}
	return targets;
}
}