#ifndef ENTITIES_H
#define ENTITIES_H

#include <set>
#include <map>
#include <vector>

namespace RECS {
	using EntityID = unsigned short;

	class IEntity
	{
	public:
		EntityID entityID;
	private:
		static EntityID IDCounetr;
	private:
		static std::set<EntityID> freeIDs;
	public:
		IEntity()
		{
			if (freeIDs.empty())
			{
				entityID = ++IDCounetr;
			}
			else
			{
				entityID = *freeIDs.begin();
				freeIDs.erase(freeIDs.begin());
			}
		}
		virtual ~IEntity()
		{
			freeIDs.insert(this->entityID);
		}

		template<class T, class ... P>
		void AddComponent(P&&... params)
		{
			ComponentContainer::template AddComponent<T>(entityID, std::forward<P>(params) ...);
		}

		template<class T>
		void DeleteComponent()
		{
			ComponentContainer::template DeleteComponent<T>(entityID);
		}

		template<class T>
		T* GetComponent()
		{
			return ComponentContainer::template GetComponent<T>(entityID);
		}
	};

	EntityID IEntity::IDCounetr = 0;
	std::set<EntityID> IEntity::freeIDs;

	class EntityContainer
	{
	public:
		static std::map<EntityID, IEntity*> m_entityContainer;
	public:
		static IEntity* CreateEntity();
		static std::vector<IEntity*> GetGroupOfEntities(std::vector<EntityID> targetIDs);
	};

	std::map<EntityID, IEntity*> EntityContainer::m_entityContainer;

	IEntity* EntityContainer::CreateEntity()
	{
		IEntity* entity = new IEntity();
		m_entityContainer[entity->entityID] = entity;
		return entity;
	}

	std::vector<IEntity*> EntityContainer::GetGroupOfEntities(std::vector<EntityID> targetIDs)
	{
		std::vector<IEntity*> vec;
		for (auto &i : targetIDs)
		{
			vec.push_back(m_entityContainer[i]);
		}
		return vec;
	}
}
#endif // !ENTITIES_H
