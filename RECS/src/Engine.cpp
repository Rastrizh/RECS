#include "Components.h"
#include "Entities.h"
#include "Events/Event.h"
#include "Engine.h"

namespace RECS {
auto Engine::instance() -> Engine &
{
	static Engine instance;
	return instance;
}

auto Engine::CreateEntity() -> Entity *
{
	auto e = m_pEntityContainer->CreateEntity();
	
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
auto Engine::GetGroup(const std::list<ComponentType>& componentTypeIDs) -> std::vector<Entity *>
{
	return m_pEntityContainer->GetGroupOfEntities(componentTypeIDs);
}
void Engine::ComponentAdded(Entity * e, ComponentType componentType)
{
	m_pEntityContainer->m_ComponentLists[e].push_back(componentType);
	OnEntityChanged(e);
}

void Engine::ComponentRemoved(Entity * e, ComponentType componentType)
{
	m_pEntityContainer->m_ComponentLists[e].remove(componentType);
	OnEntityChanged(e);
}
}