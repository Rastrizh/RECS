#include "Components.h"
#include "ComponentContainer.h"
#include "Entities.h"
#include "Engine.h"

namespace RECS {
	auto ComponentContainer::instance() -> ComponentContainer &
	{
		static ComponentContainer m_instance;
		return m_instance;
	}

	ComponentContainer::ComponentContainer()
	{
		Engine::instance().OnEntityDestroyed += [this](Entity * e) {

			OnEntityDeleted(e);
		};
	}

	ComponentContainer::~ComponentContainer()
	{
	}
	void ComponentContainer::OnEntityDeleted(Entity * e)
	{
		for (auto &ct : container)
		{
			if (ct.second.find(e->entityID) != ct.second.end())
			{
				ct.second.erase(e->entityID);
				break;
			}
		}
	}
}