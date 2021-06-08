#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include <map>
#include <utility>
#include <typeinfo>
#include "Components.h"
#include "memory/StackAllocator.h"
#include "memory/MemoryManager.h"
#include "memory/ChunkAllocator.h"
#include "RECSTypes.h"

#define MAX_CONTAINER_COUNT 64

namespace RECS{
class ComponentManager
{
private:
	class IComponentContainer 
	{
	public:
		virtual void erase(IComponent*) = 0;
	};

	template<class T>
	class ComponentContainer : public IComponentContainer
	{
	private:
		std::mutex m_compContainer_mutex;
		size_t m_size = sizeof(T) * MAX_ENTITY_COUNT;
		size_t m_used = 0;
		memory::ChunkAllocator<T> m_container_allocator;

	public:
		ComponentContainer(void* ptr)
			:
			m_container_allocator(ptr, sizeof(T) * MAX_ENTITY_COUNT, "Component container")
		{
			assert(ptr && "Pointer is nullptr");
			//RINFO("Component container of {}", typeid(T).name());
		}
		template<class ...P>
		IComponent* Emplace(P&&... params)
		{
			//std::lock_guard<std::mutex> lock(m_compContainer_mutex);

			void* place = m_container_allocator.alloc(sizeof(T));
			IComponent* component = nullptr;
			if constexpr (sizeof...(P) > 0)
				component = new(place) T{ {}, std::forward<P>(params)... };
			else
				component = new(place) T(std::forward<P>(params)...);
			m_used += sizeof(T);
			//RINFO("\nComponent container of {} {} {} {} {} {}", typeid(T).name(), "Emplace",
			//	"\nline: ", m_container_allocator.line.m_stats.ToString(),
			//	"\nPool: ", m_container_allocator.pool.m_stats.ToString());
			return component;
		}
		virtual void erase(IComponent* component) final
		{
			//std::lock_guard<std::mutex> lock(m_compContainer_mutex);
			m_container_allocator.dealloc((T*)component);
			m_used -= sizeof(T);
			//RINFO("\nComponent container of {} {} {} {} {} {}", typeid(T).name(), "Erase", 
			//	"\nline: ", m_container_allocator.line.m_stats.ToString(), 
			//	"\nPool: ", m_container_allocator.pool.m_stats.ToString());
		}
		T operator[](const entityID& eid) { return m_container_allocator[eid]; }
	};

public:
	static std::mutex s_compManager_mutex;

	static event<const entityID&, const ComponentTypeID&, IComponent*> OnComponentAdded;
	static event<const entityID&, const ComponentTypeID&> OnComponentRemoved;

private:
	static memory::StackAllocator m_compManager_allocator;
	static std::map<ComponentTypeID, IComponentContainer*> m_component_containers;

public:
	static void DeleteEntity(const std::map<ComponentTypeID, IComponent*>& comps)
	{
		//std::lock_guard<std::mutex> lock(s_compManager_mutex);
		IComponentContainer* ret;
		for (const auto &c : comps)
		{
			auto range = m_component_containers.equal_range(c.first);
			if (range.first == range.second)
				ret = nullptr;
			else
				ret = (--range.second)->second;
			ret->erase(c.second);
		}
	}
	template<class T, class ...P>
	static IComponent* AddComponent(P&&... params)
	{
		IComponent* component = GetComponentContainer<T>()->Emplace(std::forward<P>(params)...);
		
		return component;
	}
	template<class T>
	static void DeleteComponent(const ComponentTypeID& id, IComponent* component)
	{
		GetComponentContainer<T>()->erase(dynamic_cast<T>(component));
	}

	//template<class T>
	//static T GetComponent(const entityID& eid)
	//{
	//	ComponentContainer<T>* container = dynamic_cast<ComponentContainer<T>*>(m_component_containers[T::GetTypeID()]);
	//	return container[eid];
	//}

private:
	template<class T>
	static ComponentContainer<T>* GetComponentContainer()
	{
		ComponentContainer<T>* ret = dynamic_cast<ComponentContainer<T>*>(m_component_containers[T::GetTypeID()]);
		if(!ret)
			ret = CreateComponentContainer<T>();
		return ret;
	}

	template<class T>
	static ComponentContainer<T>* CreateComponentContainer()
	{
		//std::lock_guard<std::mutex> lock(s_compManager_mutex);
		auto ret = new ComponentContainer<T>(m_compManager_allocator.allocate(sizeof(T) * MAX_ENTITY_COUNT, alignof(T)));
		m_component_containers[T::GetTypeID()] = (IComponentContainer*)ret;
		return ret;
	}
}; // Class ComponentManager

memory::StackAllocator ComponentManager::m_compManager_allocator{
	memory::MemoryManager::NewMemoryUser(typeid(ComponentManager).name(), MAX_CONTAINER_COUNT * MEBIBYTE), MAX_CONTAINER_COUNT * MEBIBYTE };

std::map<ComponentTypeID, ComponentManager::IComponentContainer*> ComponentManager::m_component_containers;

std::mutex ComponentManager::s_compManager_mutex;

event<const entityID&, const ComponentTypeID&, IComponent*>	ComponentManager::OnComponentAdded;
event<const entityID&, const ComponentTypeID&>				ComponentManager::OnComponentRemoved;

}//namespace RECS

#endif // !COMPONENT_MANAGER_H