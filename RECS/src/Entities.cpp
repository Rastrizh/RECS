#include "Entities.h"
#include "Components.h"

namespace RECS {

IEntity::IEntity()
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

IEntity::~IEntity()
{
	freeIDs.insert(this->entityID);
}

void IEntity::DeleteComponent()
{
	ComponentContainer::instance().DeleteComponent(entityID);
}

auto EntityContainer::instance()->EntityContainer&
{
	static EntityContainer instance;
	return instance;
}

EntityID IEntity::IDCounetr = 0;
std::set<EntityID> IEntity::freeIDs;

}