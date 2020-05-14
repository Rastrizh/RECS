#include "Components.h"
#include "ComponentContainer.h"

namespace RECS {
	auto ComponentContainer::instance() -> ComponentContainer &
	{
		static ComponentContainer m_instance;
		return m_instance;
	}

	ComponentContainer::~ComponentContainer()
	{
		for (auto &p : container)
		{
			delete p.second;
			p.second = nullptr;
		}
	}
}