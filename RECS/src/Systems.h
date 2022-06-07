#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <set>
#include "RECSTypes.h"

namespace RECS {
	class ISystem
	{
	public:
		float m_deltaTime = 1.F / 61.F;
	public:
		virtual void Update(float deltaTime) = 0;
	};
}

#endif // !SYSTEMS_H
