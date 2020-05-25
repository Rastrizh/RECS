#include "Components.h"
#include "ComponentContainer.h"

#include "Entities.h"

namespace RECS {

Entity::Entity()
{
	if (freeIDs.empty())
	{
		entityID = ++IDCounetr;
	}
	else
	{
		entityID = *freeIDs.begin();
		freeIDs.erase(freeIDs.begin());
	}
}

Entity::~Entity()
{
	freeIDs.insert(this->entityID);
}
std::list<ComponentType>& Entity::GetEntityComponentTypes()
{
	return EntityContainer::instance().GetEntityComponentTypes(this);
}
EntityID Entity::IDCounetr = 0;
std::set<EntityID> Entity::freeIDs;
}