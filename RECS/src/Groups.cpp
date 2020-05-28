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
	{
		return;
	}
	m_entities.erase(deleted);
}

void Group::AddOrRemoveChangedEntity(Entity *e)
{
	std::lock_guard<std::mutex> Lock(m_groupLocker);
 	m_groupSignature.sort();
	auto entityComponentTypes = m_EngineInstance->GetEntityComponentTypes(e);
	entityComponentTypes.sort();
	if (Engine::IsIntersect(entityComponentTypes, m_groupSignature) == m_groupSignature)
	{
		if (std::find(m_entities.begin(), m_entities.end(), e) == m_entities.end())
		{
			AddEntity(e);
		}
	}
	else
	{
		if (std::find(m_entities.begin(), m_entities.end(), e) == m_entities.end())
		{
			return;
		}
		RemoveEntity(e);
	}
}

auto Group::GetSignature() ->ComponentTypeIDList&
{
	m_groupSignature.sort();
	return m_groupSignature;
}

auto Group::GetEntities() ->std::vector<Entity*>&
{
	return m_entities;
}

Group::Group(ComponentTypeIDList&& groupSignature)
{
	m_EngineInstance = Engine::instance();
	m_groupSignature = groupSignature;
	m_entities = m_EngineInstance->GetGroupOfEntities(std::move(groupSignature));
}
}