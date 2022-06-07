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
	static Entity*	CreateEntity();
	static Entity*	GetEntity(entityID eid);
	static void			DeleteEntity(Entity* e);

	static inline bool		IsUpdateble(entityID eid) { return s_entityManager_allocator[eid]->isUpdateble; }
	static inline size_t	EntityCount() { return alive; }
	static inline size_t	TotalEntities() { return total_entities; }
	static void				DeleteAll() { s_entityManager_allocator.clear(); }
};

Entity* EntityManager::CreateEntity()
{
	if (IDProvider<Entity>::freeID.empty())
		total_entities++;
	alive++;
	return  new(s_entityManager_allocator.alloc()) Entity();
}

void EntityManager::DeleteEntity(Entity* e)
{
	e->isUpdateble = false;
	e->isDrawable = false;
	s_entityManager_allocator.dealloc(e);
	alive--;
}

Entity* EntityManager::GetEntity(entityID eid)
{
	if (IDProvider<Entity>::isDeletedID(eid))
		return nullptr;
	return s_entityManager_allocator[eid];
}

memory::ChunkAllocator<Entity> EntityManager::s_entityManager_allocator{
	memory::MemoryManager::NewMemoryUser(typeid(EntityManager).name(), sizeof(Entity) * MAX_ENTITY_COUNT),
	sizeof(Entity) * MAX_ENTITY_COUNT,
	"Entity manager" };

size_t EntityManager::total_entities = 0;
size_t EntityManager::alive = 0;

}
#endif // ENTITY_MANAGER_H