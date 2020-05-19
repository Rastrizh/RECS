#include "Components.h"
#include "Entities.h"
#include "Events/Event.h"
#include "Engine.h"
#include "Groups.h"

RECS::Group::~Group()
{

}

void RECS::Group::AddEntity(Entity*)
{

}

void RECS::Group::RemoveEntity(Entity*)
{

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
