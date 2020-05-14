#ifndef ENTITIES_H
#define ENTITIES_H

#include <set>
#include "Events/Event.h"

namespace RECS {
	class EntityContainer;

	using EntityID = unsigned long long;

	class Entity
	{
	public:
		EntityID entityID;

	private:
		static EntityID IDCounetr;
		static std::set<EntityID> freeIDs;

	public:
		//static event<ComponentType> OnComponentAdded;
		//static event<ComponentType> OnComponentRemoved;

	public:
		Entity();
		~Entity();

		template<typename T>
		void DeleteComponent()
		{
			ComponentContainer::instance().DeleteComponent<T>(entityID);
			//OnComponentRemoved(T::GetTypeID());
		}

		template<typename T, class ... P>
		void AddComponent(P&&... params)
		{
			ComponentContainer::instance().AddComponent<T>(entityID, std::forward<P>(params) ...);
			EntityContainer::instance().m_ComponentLists[this].push_back(T::GetTypeID());
			//OnComponentAdded(T::GetTypeID());
		}

		template<typename T>
		auto GetComponent() ->T*
		{
			return ComponentContainer::instance().GetComponent<T>(entityID);
		}

		template<typename T>
		auto HasComponent() ->bool
		{
			auto comp = EntityContainer::instance().m_ComponentLists.find(this);
			auto it = std::find(comp->second.begin(), comp->second.end(), T::GetTypeID());

			if (it != comp->second.end())
				return true;

			return false;
		}
	};
}
#endif // !ENTITIES_H
