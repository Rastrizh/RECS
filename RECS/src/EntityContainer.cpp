#include "Components.h"
#include "Entities.h"
#include "EntityContainer.h"

namespace RECS {
	auto EntityContainer::instance()->EntityContainer&
	{
		static EntityContainer instance;
		return instance;
	}

	EntityID Entity::IDCounetr = 0;
	std::set<EntityID> Entity::freeIDs;
}