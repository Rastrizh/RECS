#include "Components.h"
#include "ComponentContainer.h"
#include "EntityContainer.h"
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

}