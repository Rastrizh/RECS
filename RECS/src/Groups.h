#ifndef GROUPS_H
#define GROUPS_H

namespace RECS {
	class Group
	{
	private:
		std::list<ComponentType> m_groupSignature;
		std::vector<Entity*> m_entities;

	public:
		Group(std::list<ComponentType>&& groupSignature);
		~Group();

		void AddEntity(Entity*);
		void RemoveEntity(Entity*);
		auto GetEntities()->std::vector<Entity*>&;

	};
}

#endif // GROUPS_H
