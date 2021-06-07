#ifndef ENTITY_H
#define ENTITY_H

#include "IDProvider.h"
#include "Events/Event.h"
#include "ComponentManager.h"

namespace RECS {

class Engine;
class IComponent;

class Entity
{
public:
	entityID EntityID;
	bool isUpdateble;

public:
	Entity()
	{
		EntityID = IDProvider<Entity>::Get();
		isUpdateble = true;
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
		//ComponentManager::OnComponentAdded(this->EntityID, T::GetTypeID(), new_component);
	}
	template<typename T>
	void DeleteComponent()
	{
		ComponentManager::DeleteComponent<T>(T::GetTypeID(), GetComponent<T>(EntityID));
		ComponentManager::OnComponentRemoved(this->EntityID, T::GetTypeID());
	}
	template<class T>
	T* GetComponent()
	{
		for(auto & f : ComponentManager::OnComponentAdded().m_Futures)
			f.wait();
		return ComponentManager::getComponent<T>(EntityID);
	}
	template<class T>
	auto HasComponent()
	{
		return Engine::hasComponent<T>(EntityID);
	}
};
}
#endif // !ENTITY_H
