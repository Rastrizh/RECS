#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include <unordered_map>
#include <utility>
#include <typeinfo>
#include "Components.h"
#include "memory/StackAllocator.h"
#include "memory/MemoryManager.h"
#include "memory/ChunkAllocator.h"
#include "RECSTypes.h"

#define CONTAINER_CHUNK_SIZE 100
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
		size_t m_size = sizeof(T) * CONTAINER_CHUNK_SIZE;
		size_t m_used = 0;
		memory::ChunkAllocator<T> m_container_allocator;

	public:
		ComponentContainer(void* ptr)
			:
			m_container_allocator(ptr, m_size, "Component container")
		{
			assert(ptr && "Pointer is nullptr");
			RINFO("Component container of {}", typeid(T).name());
		}
		template<class ...P>
		IComponent* Emplace(P&&... params)
		{
			if (m_used + sizeof(T) > m_size)
				grow();

			void* place = m_container_allocator.alloc(sizeof(T));
			if (!place)
			{
				place = grow();
			}
			IComponent* component = new(place) T(std::forward<P>(params)...);
			m_used += sizeof(T);
			RINFO("\nComponent container of {} {} {} {} {} {}", typeid(T).name(), "Emplace",
				"\nline: ", m_container_allocator.line.m_stats.ToString(),
				"\nPool: ", m_container_allocator.pool.m_stats.ToString());
			return component;
		}
		virtual void erase(IComponent* component) final
		{
			m_container_allocator.dealloc((T*)component);
			m_used -= sizeof(T);
			RINFO("\nComponent container of {} {} {} {} {} {}", typeid(T).name(), "Erase", 
				"\nline: ", m_container_allocator.line.m_stats.ToString(), 
				"\nPool: ", m_container_allocator.pool.m_stats.ToString());
		}
	private:
		void* grow()
		{
			auto new_container = ComponentManager::CreateComponentContainer<T>();
			return new_container->m_container_allocator.alloc(sizeof(T));
		}
	}; // class ComponentContainer

private:
	static memory::StackAllocator m_compManager_allocator;
	static std::unordered_multimap<ComponentTypeID, IComponentContainer*> m_component_containers;

public:
	static void DeleteEntity(const std::map<ComponentTypeID, IComponent*>& comps)
	{
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

private:
	template<class T>
	static ComponentContainer<T>* GetComponentContainer()
	{
		ComponentContainer<T>* ret;
		auto range = m_component_containers.equal_range(T::GetTypeID());
		if (range.first == range.second)
			ret = CreateComponentContainer<T>();
		else
		{
			ret = (ComponentContainer<T>*)(--range.second)->second;
			if (!ret)
			{
				ret = CreateComponentContainer<T>();
			}
		}
		return ret;
	}

	template<class T>
	static ComponentContainer<T>* CreateComponentContainer()
	{
		auto ret = new ComponentContainer<T>(m_compManager_allocator.allocate(sizeof(T) * CONTAINER_CHUNK_SIZE, alignof(T)));
		m_component_containers.insert({ T::GetTypeID(), (IComponentContainer*)ret });
		return ret;
	}
}; // Class ComponentManager

memory::StackAllocator ComponentManager::m_compManager_allocator{
	memory::MemoryManager::NewMemoryUser(typeid(ComponentManager).name(), MAX_CONTAINER_COUNT * MEBIBYTE), MAX_CONTAINER_COUNT * MEBIBYTE };

std::unordered_multimap<ComponentTypeID, ComponentManager::IComponentContainer*> ComponentManager::m_component_containers;

}//namespace RECS

#endif // !COMPONENT_MANAGER_H