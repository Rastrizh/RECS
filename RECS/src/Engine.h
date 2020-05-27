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
		std::map<std::list<ComponentType>, Group*> m_groups;
		mutable std::mutex m_groupsLocker;
	private:
		Engine();

	public:
		static auto instance()->Engine*;
		~Engine() = default;

		auto CreateEntity()->Entity*;
		void KillEntity(Entity* e);
		void KillAllEntities();
		auto GetGroup(std::list<ComponentType>&& componentTypeIDs)->Group*;
		
		RECS::event<Entity*> OnEntityCreated;
		RECS::event<Entity*> OnEntityDestroyed;

		void ComponentRemoved(Entity* e, ComponentType componentType);
		void ComponentAdded(Entity* e, ComponentType componentType);

		auto GetGroupOfEntities(std::list<ComponentType>&& componentTypeIDs)->std::vector<Entity*>;
		auto GetEntityComponentTypes(Entity* e) ->std::list<ComponentType>&;

		static auto IsIntersect(std::list<ComponentType>& inWhat, std::list<ComponentType>& What)->std::list<ComponentType>;
	};
}

#endif // !ENGINE_H
