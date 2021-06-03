#ifndef ENTITY_H
#define ENTITY_H

#include <set>
#include "RECSTypes.h"
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
	static entityID IDCouneter;

	event<entityID, ComponentTypeID, IComponent*> OnComponentAdded;
	event<entityID, ComponentTypeID> OnComponentRemoved;

	event<Entity*> OnEntityCreated;
	event<entityID> OnEntityDestroyed;

private:
	static std::set<entityID> freeIDs;
public:
	Entity()
	{
		if (freeIDs.empty())
		{
			EntityID = ++IDCouneter;
		}
		else
		{
			EntityID = *freeIDs.begin();
			freeIDs.erase(freeIDs.begin());
		}
		isUpdateble = false;
	}
	Entity(const Entity& e)
		: EntityID(e.EntityID), isUpdateble(e.isUpdateble)
	{
	}
	~Entity()
	{
		//freeIDs.insert(this->EntityID);
	}

	template<typename T, class ... P>
	void AddComponent(P&&... params)
	{
		IComponent* new_component = ComponentManager::AddComponent<T>(std::forward<P>(params)...);
		OnComponentAdded(this->EntityID, T::GetTypeID(), new_component);
	}
	template<typename T>
	void DeleteComponent()
	{
		ComponentManager::DeleteComponent<T>(T::GetTypeID(), GetComponent<T>(EntityID));
		OnComponentRemoved(this->EntityID, T::GetTypeID());
	}
	template<class T>
	T* GetComponent()
	{
		for(auto & f : OnComponentAdded.m_Futures)
			f.wait();
		return Engine::getComponent<T>(EntityID);
	}
	template<class T>
	auto HasComponent()
	{
		return Engine::hasComponent<T>(EntityID);
	}
};

entityID Entity::IDCouneter = 0;
std::set<entityID> Entity::freeIDs;
}
#endif // !ENTITY_H
