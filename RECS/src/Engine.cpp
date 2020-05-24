#include "Entities.h"
#include "Events/Event.h"
#include "Engine.h"
#include "EntityContainer.h"
#include "Components.h"
#include "ComponentContainer.h"

namespace RECS {
auto Engine::instance() -> Engine &
{
	static Engine instance;
	return instance;
}

Engine::Engine()
{
}

auto Engine::CreateEntity() -> Entity *
{
	auto e = EntityContainer::instance().CreateEntity();
	
	e->OnComponentAdded += [this](Entity* e, ComponentType componentType)
	{
		ComponentAdded(e, componentType);
	};

	e->OnComponentRemoved += [this](Entity* e, ComponentType componentType)
	{
		ComponentRemoved(e, componentType);
	};
	
	OnEntityCreated(e);
	return e;
}
void Engine::KillEntity(Entity* e)
{
	std::list<ComponentType> entityComponents = e->GetGetEntityComponentTypes();
	m_groups[entityComponents]->OnEntityDeleted(e, entityComponents);
	OnEntityDestroyed(e);
}
void Engine::KillAllEntities()
{
}
auto Engine::GetGroup(std::list<ComponentType>&& componentTypeIDs) -> Group*
{
	if (m_groups.find(componentTypeIDs) == m_groups.end())
	{
		Group* group = new Group(std::move(componentTypeIDs));
		group->OnEntityChanged += [group](Entity* e) {
			group->AddOrRemoveChangedEntity(e);
		};
		group->OnEntityDeleted += [group](Entity* e, const std::list<ComponentType>& componentTypes) {
			for (auto ct : componentTypes)
			{
				for (auto c : ComponentContainer::instance().container[ct])
				{
					if (e->entityID == c.first)
					{
						ComponentContainer::instance().container[ct].erase(e->entityID);
						group->RemoveEntity(e);
						break;
					}
				}
			}
		};

		m_groups[componentTypeIDs] = group;
		return group;
	}
	return m_groups[componentTypeIDs];
}
void Engine::ComponentAdded(Entity * e, ComponentType componentType)
{
	EntityContainer::instance().m_ComponentLists[e].push_back(componentType);
	for (auto g : m_groups)
		g.second->OnEntityChanged(e);
}

void Engine::ComponentRemoved(Entity * e, ComponentType componentType)
{
	EntityContainer::instance().m_ComponentLists[e].remove(componentType);
	for (auto g : m_groups)
		g.second->OnEntityChanged(e);
}
}