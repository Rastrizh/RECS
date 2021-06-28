#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include <map>
#include <utility>
#include <typeinfo>

#include "RECSTypes.h"
#include "Events/Event.h"
#include "Components.h"
#include "Entities.h"
#include "memory/StackAllocator.h"
#include "memory/MemoryManager.h"
#include "memory/ChunkAllocator.h"

#define MAX_CONTAINER_COUNT 64

namespace RECS{

class Entity;

class ComponentManager
{
protected:
	class IComponentContainer 
	{
	public:
		virtual void erase(const componentID&) = 0;
	};

	template<class T>
	class ComponentContainer : public IComponentContainer
	{
	private:
		std::mutex m_compContainer_mutex;
		size_t m_capacity = sizeof(T) * MAX_ENTITY_COUNT;
		size_t m_size = 0;
		memory::ChunkAllocator<T> m_container_allocator;

	public:
		ComponentContainer(void* ptr)
			:
			m_container_allocator(ptr, sizeof(T) * MAX_ENTITY_COUNT, "Component container")
		{
			assert(ptr && "Pointer is nullptr");
			RINFO("Create component container of {}", typeid(T).name());
		}
		template<class ...P>
		IComponent* emplace(P&&... params)
		{
			std::lock_guard<std::mutex> lock(m_compContainer_mutex);

			void* place = m_container_allocator.alloc();
			IComponent* component = nullptr;

			if constexpr (sizeof...(P) > 0)
				component = new(place) T{ {}, std::forward<P>(params)... };
			else
				component = new(place) T(std::forward<P>(params)...);

			m_size++;

			return component;
		}
		virtual void erase(const componentID& cid) final
		{
			std::lock_guard<std::mutex> lock(m_compContainer_mutex);
			m_container_allocator.dealloc(m_container_allocator[cid]);

			m_size--;
		}

	public:
		size_t size() { return m_size; }
		size_t capacity() { return m_capacity; }
		T* get(const componentID& cid) { return m_container_allocator[cid]; }
		T* operator[](const componentID& cid) { return m_container_allocator[cid]; }
	};

private:
	static memory::StackAllocator m_compManager_allocator;
	static std::map<ComponentTypeID, IComponentContainer*> m_component_containers;

public:
	static event<Entity*, const ComponentTypeID&, IComponent*> OnComponentAdded;
	static event<Entity*, const ComponentTypeID&> OnComponentRemoved;

public:
	static void DeleteEntityComponents(Entity* e);
	static void Clear();

	template<class T, class ...P>
	static IComponent* AddComponent(P&&... params)
	{
		IComponent* component = GetComponentContainer<T>()->emplace(std::forward<P>(params)...);
		
		return component;
	}
	template<class T>
	static void DeleteComponent(const ComponentTypeID& id, const componentID& cid)
	{
		GetComponentContainer<T>()->erase(cid);
	}
	template<class T>
	static T* GetComponent(const componentID& cid)
	{
		ComponentContainer<T>* container = dynamic_cast<ComponentContainer<T>*>(m_component_containers[T::GetTypeID()]);
		return container->get(cid);
	}

private:
	template<class T>
	static ComponentContainer<T>* CreateComponentContainer()
	{
		auto ret = new ComponentContainer<T>(m_compManager_allocator.allocate(sizeof(T) * MAX_ENTITY_COUNT, alignof(T)));
		m_component_containers[T::GetTypeID()] = (IComponentContainer*)ret;
		return ret;
	}
	template<class T>
	static ComponentContainer<T>* GetComponentContainer()
	{
		ComponentContainer<T>* ret = dynamic_cast<ComponentContainer<T>*>(m_component_containers[T::GetTypeID()]);
		if(!ret)
			ret = CreateComponentContainer<T>();
		return ret;
	}
}; // Class ComponentManager

void ComponentManager::DeleteEntityComponents(Entity* e)
{
	for (const auto& c : e->getComponentsTable())
	{
		m_component_containers[c.first]->erase(c.second);
	}
}

void ComponentManager::Clear()
{
	m_compManager_allocator.clear();
}

memory::StackAllocator ComponentManager::m_compManager_allocator{
	memory::MemoryManager::NewMemoryUser(typeid(ComponentManager).name(), 2 * MAX_CONTAINER_COUNT * MEBIBYTE), 2 * MAX_CONTAINER_COUNT * MEBIBYTE };

std::map<ComponentTypeID, ComponentManager::IComponentContainer*> ComponentManager::m_component_containers;

event<Entity*, const ComponentTypeID&, IComponent*>	ComponentManager::OnComponentAdded;
event<Entity*, const ComponentTypeID&>				ComponentManager::OnComponentRemoved;

}//namespace RECS

#endif // !COMPONENT_MANAGER_H