#include "Components.h"
#include "ComponentContainer.h"
#include "Entities.h"
#include "Engine.h"

namespace RECS {
	auto ComponentContainer::instance() -> ComponentContainer *
	{
		static auto *m_instance = new ComponentContainer();
		return m_instance;
	}

	ComponentContainer::ComponentContainer()
	{
		m_EngineInstance = Engine::instance();
		m_EngineInstance->OnEntityDestroyed += [this](Entity * e) {

			OnEntityDeleted(e);
		};
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