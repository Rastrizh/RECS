#include "Components.h"

auto RECS::ComponentContainer::instance() -> ComponentContainer &
{
	static ComponentContainer m_instance;
	return m_instance;
}

RECS::ComponentContainer::~ComponentContainer()
{
	for (auto &p : container)
	{
		delete p.second;
		p.second = nullptr;
	}
}

void RECS::ComponentContainer::DeleteComponent(EntityID ownerId)
{
	container.erase(ownerId);
}
