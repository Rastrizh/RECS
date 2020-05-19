#include "Components.h"
#include "Entities.h"
#include "Events/Event.h"
#include "Groups.h"
#include "Engine.h"

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
void Engine::KillEntity()
{

}
void Engine::KillAllEntities()
{
}
auto Engine::GetGroup(std::list<ComponentType>&& componentTypeIDs) -> Group*
{
	if (m_groups[componentTypeIDs] == nullptr)
	{
		Group* group = new Group(std::move(componentTypeIDs));
		m_groups[componentTypeIDs] = group;
		return group;
	}
	return m_groups[componentTypeIDs];
}
void Engine::ComponentAdded(Entity * e, ComponentType componentType)
{
	EntityContainer::instance().m_ComponentLists[e].push_back(componentType);
	OnEntityChanged(e);
}

void Engine::ComponentRemoved(Entity * e, ComponentType componentType)
{
	EntityContainer::instance().m_ComponentLists[e].remove(componentType);
	OnEntityChanged(e);
}
}