#include "Entities.h"
#include "Groups.h"
#include "Engine.h"
#include "EntityContainer.h"

namespace RECS {
void Group::AddEntity(Entity* e)
{
	m_entities.push_back(e);
}

void Group::RemoveEntity(Entity* e)
{
	auto deleted = std::find(m_entities.begin(), m_entities.end(), e);
	if (deleted == m_entities.end())
		return;
	m_entities.erase(deleted);
}

void Group::AddOrRemoveChangedEntity(Entity *e)
{
	for (auto & c : m_groupSignature)
	{
		if (e->HasComponent(c))
		{
			continue;
		}
		else
		{
			if (std::find(m_entities.begin(), m_entities.end(), e) == m_entities.end())
				return;
			RemoveEntity(e);
			return;
		}
	}
	AddEntity(e);
}

auto Group::GetEntities() ->std::vector<Entity*>&
{
	return m_entities;
}

Group::Group(std::list<ComponentType>&& groupSignature)
{
	m_groupSignature = groupSignature;
	m_entities = EntityContainer::instance().GetGroupOfEntities(std::move(groupSignature));
}
}