#ifndef ENTITY_CONTAINER_H
#define ENTITY_CONTAINER_H

#include <set>
#include <map>
#include <unordered_map>
#include <vector>
#include "RECSTypes.h"

#define EXPAND(...) __VA_ARGS__
#define FUNC_1(MODIFIER, X) MODIFIER(X)
#define FUNC_2(MODIFIER, X, ...) MODIFIER(X), EXPAND(FUNC_1(MODIFIER, __VA_ARGS__))
#define FUNC_3(MODIFIER, X, ...) MODIFIER(X), EXPAND(FUNC_2(MODIFIER, __VA_ARGS__))
#define FUNC_4(MODIFIER, X, ...) MODIFIER(X), EXPAND(FUNC_3(MODIFIER, __VA_ARGS__))
#define FUNC_5(MODIFIER, X, ...) MODIFIER(X), EXPAND(FUNC_4(MODIFIER, __VA_ARGS__))
#define FUNC_6(MODIFIER, X, ...) MODIFIER(X), EXPAND(FUNC_5(MODIFIER, __VA_ARGS__))
#define GET_MACRO(_1, _2, _3, _4, _5, _6, NAME, ...) NAME
#define FOR_EACH(MODIFIER, ...) EXPAND(GET_MACRO(__VA_ARGS__, FUNC_6, FUNC_5, FUNC_4, FUNC_3, FUNC_2, FUNC_1)(MODIFIER, __VA_ARGS__))

#define COMPONENT_GET_TYPE_ID(COMPONENT_CLASS) COMPONENT_CLASS::GetTypeID()
#define CREATE_TARGET_ENTITY_ID_LIST(...) (std::list<RECS::ComponentType>({ FOR_EACH(COMPONENT_GET_TYPE_ID, __VA_ARGS__) }))

namespace RECS {
	class Entity;
	class Engine;

	class EntityContainer
	{
	public:
		std::unordered_map<EntityID, Entity*> m_entityContainer;
		std::map<Entity*, ComponentTypeIDList> m_ComponentLists;

	public:
		~EntityContainer();
		EntityContainer() = default;

	public:
		auto CreateEntity()->Entity*;
		void DeleteEntity(Entity* e);
		auto GetEntityComponentTypes(Entity* e)->ComponentTypeIDList&;
		auto GetGroupOfEntities(ComponentTypeIDList&& componentTypeIDs)->std::vector<Entity*>;
	};
}

#endif // !ENTITY_CONTAINER_H
