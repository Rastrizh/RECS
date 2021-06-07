#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Entities.h"
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
			Entity* e = new(s_entityManager_allocator.alloc(sizeof(Entity))) Entity();
			return e;
		}
		static bool IsUpdateble(entityID eid)
		{
			return s_entityManager_allocator[eid]->isUpdateble;
		}
		static void DeleteEntity(entityID eid)
		{
			s_entityManager_allocator.dealloc(s_entityManager_allocator[eid]);
		}
	};

	memory::ChunkAllocator<Entity> EntityManager::s_entityManager_allocator{
		memory::MemoryManager::NewMemoryUser(typeid(EntityManager).name(), sizeof(Entity) * MAX_ENTITY_COUNT),
		sizeof(Entity) * MAX_ENTITY_COUNT,
		"Entity manager" };
}
#endif // ENTITY_MANAGER_H