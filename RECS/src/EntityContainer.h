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
		~EntityContainer();
		static EntityContainer& instance() 
		{
			static EntityContainer m_instance;
			return m_instance;
		}

	private:
		EntityContainer() = default;

	public:
		auto CreateEntity()->Entity*;
		auto GetGroupOfEntities(std::list<ComponentType>&& componentTypeIDs)->std::vector<Entity*>;
	};
}

#endif // !ENTITY_CONTAINER_H
