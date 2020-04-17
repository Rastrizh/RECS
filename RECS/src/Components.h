#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "Entities.h"
#include <unordered_map>

namespace RECS {
	template<typename T>
	class IComponent
	{
	private:
		class ComponentContainer
		{
		private:
			std::unordered_map<EntityID, IComponent<T>*> container;
		public:
			void push(EntityID entityID, IComponent<T>* component);
			void pop(EntityID entityID);
		};
	private:
		ComponentContainer container;
	public:
		virtual void AddComponent(EntityID entityID)
		{
			container.push(entityID, this);
		}
		virtual void DeleteComponent(EntityID entityID)
		{
			container.pop(entityID);
		}
	};
	template<typename T>
	inline void IComponent<T>::ComponentContainer::push(EntityID entityID, IComponent<T>* component)
	{
		container.emplace(entityID, component);
	}
	template<typename T>
	inline void IComponent<T>::ComponentContainer::pop(EntityID entityID)
	{
		container.erase(entityID);
	}
}

#endif // !COMPONENTS_H
