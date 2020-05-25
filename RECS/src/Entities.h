#ifndef ENTITIES_H
#define ENTITIES_H

#include <set>
#include "Events/Event.h"
#include "RECSTypes.h"
#include "EntityContainer.h"

namespace RECS {

	class Entity
	{
	public:
		EntityID entityID;

	private:
		static EntityID IDCounetr;
		static std::set<EntityID> freeIDs;

	public:
		event<Entity*, ComponentType> OnComponentAdded;
		event<Entity*, ComponentType> OnComponentRemoved;

	public:
		Entity();
		~Entity();

		std::list<ComponentType>& GetEntityComponentTypes();

		template<typename T>
		void DeleteComponent()
		{
			ComponentContainer::instance().DeleteComponent<T>(entityID);
			OnComponentRemoved(this, T::GetTypeID());
		}

		template<typename T, class ... P>
		void AddComponent(P&&... params)
		{
			ComponentContainer::instance().AddComponent<T>(entityID, std::forward<P>(params) ...);
			OnComponentAdded(this, T::GetTypeID());
		}

		template<typename T>
		auto GetComponent() ->T*
		{
			return ComponentContainer::instance().GetComponent<T>(entityID);
		}

		auto HasComponent(ComponentType componentTypeId) ->bool
		{
			auto comp = EntityContainer::instance().m_ComponentLists.find(this);
			auto it = std::find(comp->second.begin(), comp->second.end(), componentTypeId);

			if (it != comp->second.end())
				return true;

			return false;
		}
	};
}
#endif // !ENTITIES_H
