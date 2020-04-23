#ifndef ENTITIES_H
#define ENTITIES_H

#include <set>

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
}
#endif // !ENTITIES_H
