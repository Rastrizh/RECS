#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Entities.h"
#include "ComponentManager.h"
#include "memory/ChunkAllocator.h"
#include "memory/MemoryManager.h"

#define MAX_ENTITY_COUNT 10000

namespace RECS {
class EntityManager
{
private:
	static memory::ChunkAllocator<Entity> s_entityManager_allocator;
	static size_t entities;
public:
	static Entity* CreateEntity()
	{
		//RINFO("Entity manager");
		entities++;
		return  new(s_entityManager_allocator.alloc()) Entity();
	}
	static void DeleteEntity(Entity* e)
	{
		s_entityManager_allocator.dealloc(e);
		entities--;
	}
	static Entity* GetEntity(entityID eid) 
	{
		if (IDProvider<Entity>::has(eid))
			return nullptr;
		return s_entityManager_allocator[eid]; 
	}
	static bool		IsUpdateble(entityID eid) { return s_entityManager_allocator[eid]->isUpdateble; }
	static size_t	EntityCount() { return entities; }
	static size_t	TotalEntities() { return s_entityManager_allocator.getElementCount(); }
	static void		DeleteAll() { s_entityManager_allocator.clear(); }
};

memory::ChunkAllocator<Entity> EntityManager::s_entityManager_allocator{
	memory::MemoryManager::NewMemoryUser(typeid(EntityManager).name(), sizeof(Entity) * MAX_ENTITY_COUNT),
	sizeof(Entity) * MAX_ENTITY_COUNT,
	"Entity manager" };

size_t EntityManager::entities = 0;
}
#endif // ENTITY_MANAGER_H