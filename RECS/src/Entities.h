#ifndef ENTITY_H
#define ENTITY_H

#include "IDProvider.h"
#include "Events/Event.h"

#define MAX_ENTITY_COUNT 10000

#include <map>

namespace RECS {

class ComponentManager;
class IComponent;

class Entity
{
private:
	using componentsTable = std::map<ComponentTypeID, componentID>;
	componentsTable m_components;
public:
	entityID ID;
	bool isUpdateble;
	bool isDrawable;
	bool isAlive;

public:
	Entity()
		: ID(IDProvider<Entity>::Get()),
		isUpdateble(true),
		isDrawable(true),
		isAlive(true)
	{
	}
	Entity(const Entity& e)
		: ID(e.ID),
		isUpdateble(e.isUpdateble),
		isDrawable(true),
		isAlive(true)
	{
	}
	~Entity()
	{
		IDProvider<Entity>::Remove(this->ID);
	}

	template<typename T, class ... P>
	void AddComponent(P&&... params)
	{
		IComponent* new_component = ComponentManager::AddComponent<T>(std::forward<P>(params)...);
		((T*)new_component)->ownerID = this->ID;
		((T*)new_component)->ID = IDProvider<T>::Get();
		m_components[T::GetTypeID()] = ((T*)new_component)->ID;
		//ComponentManager::OnComponentAdded(this->ID, T::GetTypeID(), new_component);
	}
	template<typename T>
	void DeleteComponent()
	{
		ComponentManager::DeleteComponent<T>(T::GetTypeID(), m_components[T::GetTypeID()]);
		m_components.erase(T::GetTypeID());
		//ComponentManager::OnComponentRemoved(this->ID, T::GetTypeID());
	}
	template<class T>
	typename ComponentHandle<T> GetComponent()
	{
		//for(auto & f : ComponentManager::OnComponentAdded().m_Futures)
		//	f.wait();
		return ComponentHandle<T>(ComponentManager::GetComponent<T>(getComponentID<T>()));
	}
	template<class T>
	bool HasComponent() const
	{
		if (m_components.empty())
			return false;
		return m_components.find(T::GetTypeID()) != m_components.end();
	}
	template<class T, class V, class ...Types>
	bool HasComponent() const
	{
		return HasComponent<T>() && HasComponent<V, Types...>();
	}

	template<typename T>
	const componentID& getComponentID() { return m_components[T::GetTypeID()]; }
	const componentsTable& getComponentsTable() const { return m_components; }
};
}
#endif // !ENTITY_H
