#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <vector>
#include <algorithm>
#include "Entities.h"
#include "Components.h"
#include "Groups.h"

namespace RECS {
	class ISystem
	{
	protected:
		Group* m_targetGroup;
	public:
		virtual ~ISystem() = default;
		virtual void CreatePool() = 0;
		virtual void Update() = 0;
	};
}

#endif // !SYSTEMS_H
