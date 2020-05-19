#ifndef COMPONENT_CONTAINER_H
#define COMPONENT_CONTAINER_H

#include "Entities.h"
#include <map>

namespace RECS {
	class ComponentContainer
	{
	public:
		std::unordered_map<ComponentType, std::map<EntityID, IComponent*>> container;

	public:
		static auto instance()->ComponentContainer&;
		~ComponentContainer();

	private:
		ComponentContainer() = default;

	public:
		template<typename T>
		void DeleteComponent(EntityID ownerId)
		{
			container[T::GetTypeID()].erase(ownerId);
		}

		template<typename T, typename ... P>
		void AddComponent(EntityID ownerId, P&&... params)
		{
			IComponent *component = new T(std::forward<P>(params) ...);
			container[T::GetTypeID()][ownerId] = component;
		}

		template<typename T>
		auto GetComponent(EntityID ownerId) ->T*
		{
			return (T*)container[T::GetTypeID()][ownerId];
		}

	}; // Class ComponentContainer
}

#endif // !COMPONENT_CONTAINER_H
