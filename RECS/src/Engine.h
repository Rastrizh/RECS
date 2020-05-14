#ifndef ENGINE_H
#define ENGINE_H

#include "Entities.h"
#include "Components.h"

namespace RECS {
	class Engine
	{
	public:
		EntityContainer& m_entityContainerInstance = EntityContainer::instance();
		ComponentContainer& m_componentContainerInstance = ComponentContainer::instance();

	public:
		Engine() = default;
		~Engine() = default;

		auto CreateEntity()->IEntity*;
		void KillEntity();
		void KillAllEntities();
	};
}

#endif // !ENGINE_H
