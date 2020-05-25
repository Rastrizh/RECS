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
		std::map<std::list<ComponentType>, Group*> m_groups;
		std::mutex m_groupsLocker;
	private:
		Engine();

	public:
		static auto instance()->Engine&;
		~Engine() = default;

		auto CreateEntity()->Entity*;
		void KillEntity(Entity* e);
		void KillAllEntities();
		auto GetGroup(std::list<ComponentType>&& componentTypeIDs)->Group*;
		
		RECS::event<Entity*> OnEntityCreated;
		RECS::event<Entity*> OnEntityDestroyed;

		void ComponentAdded(Entity* e, ComponentType componentType);
		void ComponentRemoved(Entity* e, ComponentType componentType);
	};
}

#endif // !ENGINE_H
