#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <vector>
#include <algorithm>
#include "Entities.h"
#include "Components.h"
#include "Groups.h"
#include "Timer.h"

namespace RECS {
	class ISystem
	{
	protected:
		Group* m_targetGroup;
	public:
		float m_deltaTime = 1.F / 61.F;
	public:
		virtual ~ISystem() = default;
		virtual void CreatePool() = 0;
		virtual void Update(float deltaTime) = 0;
	};
}

#endif // !SYSTEMS_H
