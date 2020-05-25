#include "Components.h"
#include "ComponentContainer.h"
#include "Entities.h"

namespace RECS {
	auto ComponentContainer::instance() -> ComponentContainer &
	{
		static ComponentContainer m_instance;
		return m_instance;
	}

	ComponentContainer::~ComponentContainer()
	{
	}
	void ComponentContainer::OnEntityDeleted(Entity * e, const std::list<ComponentType>& componentTypes)
	{
		std::lock_guard<std::mutex> lock(m_componentContainerLocker);
		for (auto ct : componentTypes)
		{
			for (auto c : container[ct])
			{
				if (e->entityID == c.first)
				{
					container[ct].erase(e->entityID);
					break;
				}
			}
		}
	}
}