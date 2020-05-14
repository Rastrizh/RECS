#include "Components.h"
#include "Entities.h"
#include "EntityContainer.h"

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

auto EntityContainer::GetGroupOfEntities(const std::list<ComponentType>& componentTypeIDs) ->std::vector<Entity*>
{
	std::vector<Entity*> targets;
	for (auto &e : m_ComponentLists)
	{
		if (e.second == componentTypeIDs)
		{
			targets.push_back(e.first);
		}
	}
	return targets;
}

EntityID Entity::IDCounetr = 0;
std::set<EntityID> Entity::freeIDs;
}