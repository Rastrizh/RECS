#ifndef ENTITY_CONTAINER_H
#define ENTITY_CONTAINER_H

#include <set>
#include <map>
#include <unordered_map>
#include <vector>

namespace RECS {
	class EntityContainer
	{
	public:
		std::unordered_map<EntityID, Entity*> m_entityContainer;

		std::map<Entity*, std::list<ComponentType>> m_ComponentLists;
	public:
		static auto instance()->EntityContainer&;
		~EntityContainer();
	private:
		EntityContainer() = default;
	public:
		auto CreateEntity()->Entity*;
		auto GetGroupOfEntities(const std::list<ComponentType>& componentTypeIDs)->std::vector<Entity*>;
	};

	inline EntityContainer::~EntityContainer()
	{
		for (auto &p : m_entityContainer)
		{
			delete p.second;
			p.second = nullptr;
		}
	}

	inline auto EntityContainer::CreateEntity() ->Entity*
	{
		auto entity = new Entity();
		m_entityContainer[entity->entityID] = entity;
		return entity;
	}

	inline auto EntityContainer::GetGroupOfEntities(const std::list<ComponentType>& componentTypeIDs) ->std::vector<Entity*>
	{
		std::vector<Entity*> targets;
		for (auto &e : m_ComponentLists)
		{
			if (e.second == componentTypeIDs)
			{
				targets.push_back(e.first);
			}
		}
		return targets;
	}
}

#endif // !ENTITY_CONTAINER_H
