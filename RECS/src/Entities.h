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

private:
	static std::set<entityID> freeIDs;
public:
	Entity()
	{
		if (freeIDs.empty())
		{
			EntityID = IDCouneter;
			IDCouneter++;
		}
		else
		{
			EntityID = *freeIDs.begin();
			freeIDs.erase(freeIDs.begin());
		}
		isUpdateble = true;
	}
	Entity(const Entity& e)
		: EntityID(e.EntityID), isUpdateble(e.isUpdateble)
	{
	}
	~Entity()
	{
		freeIDs.insert(this->EntityID);
	}

	template<typename T, class ... P>
	void AddComponent(P&&... params)
	{
		IComponent* new_component = ComponentManager::AddComponent<T>(std::forward<P>(params)...);
		ComponentManager::OnComponentAdded(this->EntityID, T::GetTypeID(), new_component);
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
		/*for(auto & f : ComponentManager::OnComponentAdded().m_Futures)
			f.wait();*/
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
