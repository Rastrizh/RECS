#include "Components.h"
#include "Entities.h"
#include "Events/Event.h"
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
	if (std::find(m_entities.begin(), m_entities.end(), e) == m_entities.end())
		return;

	for (auto & c : m_groupSignature)
	{
		if (e->HasComponent(c))
		{
			continue;
		}
		else
		{
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