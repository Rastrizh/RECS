#ifndef GROUPS_H
#define GROUPS_H

#include "RECSTypes.h"
#include "Events/Event.h"

namespace RECS {
	class Entity;
	
	class Group
	{
	private:
		Engine *m_EngineInstance;
		ComponentTypeIDList m_groupSignature;
		std::vector<Entity*> m_entities;
		std::mutex m_groupLocker;

	public:
		explicit Group(ComponentTypeIDList&& groupSignature);
		~Group() = default;

		RECS::event<Entity*> OnEntityChanged;
		RECS::event<Entity*> OnEntityDeleted;

		void AddEntity(Entity* e);
		void RemoveEntity(Entity* e);
		void AddOrRemoveChangedEntity(Entity* e);
		auto GetSignature()->ComponentTypeIDList&;
		auto GetEntities()->std::vector<Entity*>&;

	};
}

#endif // GROUPS_H
