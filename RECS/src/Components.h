#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "Entities.h"
#include <unordered_map>
#include <typeinfo>

namespace RECS {
	class IComponent
	{
	public:
		virtual ~IComponent() = default;
	};

	template<class T>
	class Component : public IComponent
	{
	public:
		~Component() override = default;
	private:
		static const size_t STATIC_COMONENT_TYPE_ID;
	public:
		static const size_t GetTypeID() { return STATIC_COMONENT_TYPE_ID; }
	};
	template<class T>
	const size_t Component<T>::STATIC_COMONENT_TYPE_ID = typeid(T).hash_code();

	class ComponentContainer
	{
	public:
		static std::unordered_map<size_t, std::unordered_map<EntityID, IComponent*>> container;
	public:
		template<class T, class ... P>
		static void AddComponent(EntityID ownerId, P&&... params);
		
		template<class T>
		static void DeleteComponent(EntityID ownerId);

		template<class T>
		static T* GetComponent(EntityID ownerId);
	};
	std::unordered_map<size_t, std::unordered_map<EntityID, IComponent*>> ComponentContainer::container;

	template<class T, class ...P>
	inline void ComponentContainer::AddComponent(EntityID ownerId, P && ...params)
	{
		IComponent *component = new T(std::forward<P>(params) ...);
		container[T::GetTypeID()][ownerId] = component;
	}
	template<class T>
	inline void ComponentContainer::DeleteComponent(EntityID ownerId)
	{
		container[T::GetTypeID()].erase(ownerId);
	}
	template<class T>
	inline T * ComponentContainer::GetComponent(EntityID ownerId)
	{
		return dynamic_cast<T*>(container[T::GetTypeID()][ownerId]);
	}
}
#endif // !COMPONENTS_H
