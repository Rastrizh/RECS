#ifndef VIEW_H
#define VIEW_H

#include <tuple>

#include "ComponentManager.h"
#include "EntityManager.h"

namespace RECS{
template<typename ...Types>
class View
{
private:
	std::tuple<ComponentManager::ComponentContainer<Types>*...> m_targets;

public:
	View()
	{
		m_targets = std::make_tuple(ComponentManager::GetComponentContainer<Types...>());
	}

	void each(const std::function<void(Entity*, Types&...comps)>& update)
	{
		auto nentities = EntityManager::EntityCount();
		for (auto i = 0; i < nentities; i++)
		{
			auto e = EntityManager::GetEntity(i);
			if (e && e->HasComponent<Types...>())
			{
				update(e, std::make_from_tuple<Types...>(std::move(std::apply([&e](const auto... containers) ->std::tuple<Types...> {
					std::tuple<Types...> ret = std::make_tuple(containers->get(e->getComponentID())...);
						return ret;
					}, m_targets))));
			}
			else
				continue;
		}
	}
};
}
#endif // !VIEW_H
