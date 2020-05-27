#ifndef COMPONENT_CONTAINER_H
#define COMPONENT_CONTAINER_H

#include <unordered_map>
#include <map>
#include <mutex>
#include "RECSTypes.h"

namespace RECS {
	class IComponent;
	class Entity;
	class Engine;

	class ComponentContainer
	{
	private:
		Engine *m_EngineInstance;
		mutable std::mutex m_componentContainerLocker;

	public:
		std::unordered_map<ComponentType, std::map<EntityID, IComponent*>> container;

	public:
		static auto instance()->ComponentContainer*;
		~ComponentContainer();

	private:
		ComponentContainer();

	public:
		void OnEntityDeleted(Entity* e);

		template<typename T>
		void DeleteComponent(EntityID ownerId)
		{
			std::mutex m_componentContainerLocker;
			container[T::GetTypeID()].erase(ownerId);
		}

		template<typename T, typename ... P>
		void AddComponent(EntityID ownerId, P&&... params)
		{
			IComponent *component = new T(std::forward<P>(params)...);
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
