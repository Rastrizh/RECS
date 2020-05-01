#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <unordered_map>
#include <typeinfo>
#include "Entities.h"

namespace RECS {
	class IComponent
	{
	public:
		virtual ~IComponent() = default;
	}; // Class IComponent

	template<class T>
	class Component : public IComponent
	{
	public:
		~Component() override = default;
	private:
		static const size_t STATIC_COMONENT_TYPE_ID;
	public:
		static const size_t GetTypeID() { return STATIC_COMONENT_TYPE_ID; }
	}; // Class Component

	template<class T>
	const size_t Component<T>::STATIC_COMONENT_TYPE_ID = typeid(T).hash_code();

	class ComponentContainer
	{
	public:
		std::unordered_map<size_t, std::unordered_map<RECS::EntityID, IComponent*>> container;
	public:
		static ComponentContainer& instance()
		{
			static ComponentContainer* m_instance = new ComponentContainer();
			return *m_instance;
		}
	private:
		ComponentContainer() {}
	public:
		template<class T, class ... P>
		void AddComponent(EntityID ownerId, P&&... params)
		{
			IComponent *component = new T(std::forward<P>(params) ...);
			container[T::GetTypeID()][ownerId] = component;
		}
		
		template<class T>
		void DeleteComponent(EntityID ownerId)
		{
			container[T::GetTypeID()].erase(ownerId);
		}

		template<class T>
		T* GetComponent(EntityID ownerId)
		{
			return dynamic_cast<T*>(container[T::GetTypeID()][ownerId]);
		}
	}; // Class ComponentContainer
}
#endif // !COMPONENTS_H
