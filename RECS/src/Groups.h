#ifndef GROUPS_H
#define GROUPS_H

#include "RECSTypes.h"
#include "Events/Event.h"

namespace RECS {
	class Entity;
	
	class Group
	{
	private:
		std::list<ComponentType> m_groupSignature;
		std::vector<Entity*> m_entities;

	public:
		Group(std::list<ComponentType>&& groupSignature);
		~Group() = default;

		RECS::event<Entity*> OnEntityChanged;
		RECS::event<Entity*, const std::list<ComponentType>&> OnEntityDeleted;

		void AddEntity(Entity*);
		void RemoveEntity(Entity*);
		void AddOrRemoveChangedEntity(Entity*);
		auto GetSignature()->std::list<ComponentType>&;
		auto GetEntities()->std::vector<Entity*>&;

	};
}

#endif // GROUPS_H
