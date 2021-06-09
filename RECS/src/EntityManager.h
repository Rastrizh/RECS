#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Entities.h"
#include "ComponentManager.h"
#include "memory/ChunkAllocator.h"
#include "memory/MemoryManager.h"

namespace RECS {
	class EntityManager
	{
	private:
		static memory::ChunkAllocator<Entity> s_entityManager_allocator;
	public:
		static Entity* CreateEntity()
		{
			//RINFO("Entity manager");
			return  new(s_entityManager_allocator.alloc()) Entity();
		}
		static void DeleteEntity(Entity* e)
		{
			ComponentManager::DeleteEntityComponents(e);
			s_entityManager_allocator.dealloc(e);
		}
		static Entity*	GetEntity(entityID eid) { return s_entityManager_allocator[eid]; }
		static bool		IsUpdateble(entityID eid) { return s_entityManager_allocator[eid]->isUpdateble; }
		static size_t	EntityCount() { return s_entityManager_allocator.getElementCount(); }
		static void		Clear() { s_entityManager_allocator.clear(); }
	};

	memory::ChunkAllocator<Entity> EntityManager::s_entityManager_allocator{
		memory::MemoryManager::NewMemoryUser(typeid(EntityManager).name(), sizeof(Entity) * MAX_ENTITY_COUNT),
		sizeof(Entity) * MAX_ENTITY_COUNT,
		"Entity manager" };
}
#endif // ENTITY_MANAGER_H