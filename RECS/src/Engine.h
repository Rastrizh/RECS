#ifndef ENGINE_H
#define ENGINE_H
#include "EntityContainer.h"
#include "Groups.h"

namespace RECS {
	class Engine
	{
	private:
		std::map<std::list<ComponentType>, Group*> m_groups;
	private:
		Engine();

	public:
		static auto instance()->Engine&;
		~Engine() = default;

		auto CreateEntity()->Entity*;
		void KillEntity();
		void KillAllEntities();
		auto GetGroup(std::list<ComponentType>&& componentTypeIDs)->Group*;
		
		event<Entity*> OnEntityCreated;
		event<Entity*> OnEntityDestroyed;
		event<Entity*> OnEntityChanged;

		void ComponentAdded(Entity* e, ComponentType componentType);
		void ComponentRemoved(Entity* e, ComponentType componentType);
	};
}

#endif // !ENGINE_H
