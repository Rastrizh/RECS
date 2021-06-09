#ifndef ENTITY_H
#define ENTITY_H

#include "IDProvider.h"
#include "Events/Event.h"

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
	entityID EntityID;
	bool isUpdateble = true;

public:
	Entity()
		: EntityID(IDProvider<Entity>::Get())
	{
	}
	Entity(const Entity& e)
		: EntityID(e.EntityID), isUpdateble(e.isUpdateble)
	{
	}
	~Entity()
	{
		IDProvider<Entity>::Remove(this->EntityID);
	}

	template<typename T, class ... P>
	void AddComponent(P&&... params)
	{
		IComponent* new_component = ComponentManager::AddComponent<T>(std::forward<P>(params)...);
		((Component<T>*)new_component)->ownerID = this->EntityID;
		((Component<T>*)new_component)->ID = IDProvider<T>::Get();
		m_components[T::GetTypeID()] = ((Component<T>*)new_component)->ID;
		//ComponentManager::OnComponentAdded(this->EntityID, T::GetTypeID(), new_component);
	}
	template<typename T>
	void DeleteComponent()
	{
		ComponentManager::DeleteComponent<T>(T::GetTypeID(), m_components[T::GetTypeID()]);
		m_components.erase(T::GetTypeID());
		//ComponentManager::OnComponentRemoved(this->EntityID, T::GetTypeID());
	}
	template<class T>
	T* GetComponent()
	{
		for(auto & f : ComponentManager::OnComponentAdded().m_Futures)
			f.wait();
		return ComponentManager::getComponent<T>(m_components[T::GetTypeID()]);
	}
	template<class T>
	bool HasComponent()
	{
		return m_components[T::GetTypeID()];
	}
	const componentsTable& getComponentsTable() const { return m_components; }
};
}
#endif // !ENTITY_H
