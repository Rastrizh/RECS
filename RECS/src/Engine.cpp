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
	auto e = m_pEntityContainerInstance->CreateEntity();
	OnEntityCreated(e);
	return e;
}
void Engine::KillEntity()
{

}
void Engine::KillAllEntities()
{
}
}