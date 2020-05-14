#include "Components.h"
//#include "EntityContainer.h"
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
	/*auto e = EntityContainer::instance().CreateEntity();
	OnEntityCreated(e);
	return e;*/
	return nullptr;
}
void Engine::KillEntity()
{

}
void Engine::KillAllEntities()
{
}
}