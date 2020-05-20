#include "Components.h"
#include "Entities.h"
#include "Events/Event.h"
#include "Groups.h"
#include "Engine.h"

RECS::Group::~Group()
{

}

void RECS::Group::AddEntity(Entity*)
{

}

void RECS::Group::RemoveEntity(Entity*)
{

}

void RECS::Group::AddOrRemoveChangedEntity(Entity *e)
{
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

auto RECS::Group::GetEntities() ->std::vector<Entity*>&
{
	return m_entities;
}

RECS::Group::Group(std::list<ComponentType>&& groupSignature)
{
	m_groupSignature = groupSignature;
	m_entities = EntityContainer::instance().GetGroupOfEntities(std::move(groupSignature));
}
