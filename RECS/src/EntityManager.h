#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Entities.h"
#include "memory/ChunkAllocator.h"
#include "memory/MemoryManager.h"

#define MAX_ENTITY_COUNT 10000

namespace RECS {
	class EntityManager
	{
	private:
		static memory::ChunkAllocator<Entity> s_entityManager_allocator;
		static std::map<entityID, Entity*> s_entity_lookup_table;
	public:
		static Entity* CreateEntity()
		{
			//RINFO("Entity manager");
			Entity* e = new(s_entityManager_allocator.alloc(sizeof(Entity))) Entity();
			s_entity_lookup_table[e->EntityID] = e;
			return e;
		}
		static bool IsUpdateble(entityID eid)
		{
			return s_entity_lookup_table[eid]->isUpdateble;
		}
		static Entity* getEntityPtr(entityID eid)
		{
			return s_entity_lookup_table[eid];
		}
		static void DeleteEntity(entityID e)
		{
			s_entityManager_allocator.dealloc(s_entity_lookup_table[e]);
		}
	};

	std::map<entityID, Entity*> EntityManager::s_entity_lookup_table;

	memory::ChunkAllocator<Entity> EntityManager::s_entityManager_allocator{
		memory::MemoryManager::NewMemoryUser(typeid(EntityManager).name(), sizeof(Entity) * MAX_ENTITY_COUNT),
		sizeof(Entity) * MAX_ENTITY_COUNT,
		"Entity manager" };
}
#endif // ENTITY_MANAGER_H