#ifndef GROUPS_H
#define GROUPS_H

#include "RECSTypes.h"

namespace RECS {
	class Group
	{
	private:
		std::list<ComponentType> m_groupSignature;
		std::vector<Entity*> m_entities;

	public:
		Group(std::list<ComponentType>&& groupSignature);
		~Group() = default;

		event<Entity*> OnEntityChanged;
		event<Entity*> OnEntityDeleted;

		void AddEntity(Entity*);
		void RemoveEntity(Entity*);
		void AddOrRemoveChangedEntity(Entity*);
		auto GetEntities()->std::vector<Entity*>&;

	};
}

#endif // GROUPS_H
