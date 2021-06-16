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
	static size_t total_entities;
	static size_t alive;
public:
	static Entity* CreateEntity()
	{
		//RINFO("Entity manager");
		if (IDProvider<Entity>::freeID.empty())
			total_entities++;
		alive++;
		return  new(s_entityManager_allocator.alloc()) Entity();
	}
	static void DeleteEntity(Entity* e)
	{
		e->isUpdateble = false;
		e->isDrawable = false;
		s_entityManager_allocator.dealloc(e);
		alive--;
	}
	static Entity* GetEntity(entityID eid) 
	{
		if (IDProvider<Entity>::isDeletedID(eid))
			return nullptr;
		return s_entityManager_allocator[eid]; 
	}
	static bool		IsUpdateble(entityID eid) { return s_entityManager_allocator[eid]->isUpdateble; }
	static size_t	EntityCount() { return alive; }
	static size_t	TotalEntities() { return total_entities; }
	static void		DeleteAll() { s_entityManager_allocator.clear(); }
};

memory::ChunkAllocator<Entity> EntityManager::s_entityManager_allocator{
	memory::MemoryManager::NewMemoryUser(typeid(EntityManager).name(), sizeof(Entity) * MAX_ENTITY_COUNT),
	sizeof(Entity) * MAX_ENTITY_COUNT,
	"Entity manager" };

size_t EntityManager::total_entities = 0;
size_t EntityManager::alive = 0;
}
#endif // ENTITY_MANAGER_H