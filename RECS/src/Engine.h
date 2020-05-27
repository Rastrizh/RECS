#ifndef ENGINE_H
#define ENGINE_H

#include "RECSTypes.h"
#include "Events/Event.h"
#include "Groups.h"

namespace RECS {
	class Entity;

	class Engine
	{
	private:
		std::unique_ptr<EntityContainer> m_EntityContainer;
		std::map<ComponentTypeIDList, Group*> m_groups;
		mutable std::mutex m_groupsLocker;
	private:
		Engine();

	public:
		static auto instance()->Engine*;
		~Engine() = default;

		auto CreateEntity()->Entity*;
		void KillEntity(Entity* e);
		void KillAllEntities();
		auto GetGroup(ComponentTypeIDList&& componentTypeIDs)->Group*;
		
		RECS::event<Entity*> OnEntityCreated;
		RECS::event<Entity*> OnEntityDestroyed;

		void ComponentRemoved(Entity* e, ComponentType componentType);
		void ComponentAdded(Entity* e, ComponentType componentType);

		auto GetGroupOfEntities(ComponentTypeIDList&& componentTypeIDs)->std::vector<Entity*>;
		auto GetEntityComponentTypes(Entity* e) ->ComponentTypeIDList&;

		static auto IsIntersect(ComponentTypeIDList& inWhat, ComponentTypeIDList& What)->ComponentTypeIDList;
	};
}

#endif // !ENGINE_H
