#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <vector>
#include <functional>
#include "Components.h"

#define EXPAND(...) __VA_ARGS__
#define FUNC_1(MODIFIER, X) MODIFIER(X)
#define FUNC_2(MODIFIER, X, ...) MODIFIER(X), FUNC_1(MODIFIER, __VA_ARGS__)
#define FUNC_3(MODIFIER, X, ...) MODIFIER(X), FUNC_2(MODIFIER, __VA_ARGS__)
#define FUNC_4(MODIFIER, X, ...) MODIFIER(X), FUNC_3(MODIFIER, __VA_ARGS__)
#define FUNC_5(MODIFIER, X, ...) MODIFIER(X), FUNC_4(MODIFIER, __VA_ARGS__)
#define FUNC_6(MODIFIER, X, ...) MODIFIER(X), FUNC_5(MODIFIER, __VA_ARGS__)
#define GET_MACRO(_1, _2, _3, _4, _5, _6, NAME, ...) NAME
#define FOR_EACH(MODIFIER, ...) EXPAND(GET_MACRO(__VA_ARGS__, FUNC_6, FUNC_5, FUNC_4, FUNC_3, FUNC_2, FUNC_1)(MODIFIER, __VA_ARGS__))

#define COMPONENT_GET_TYPE_ID(COMPONENT_CLASS) COMPONENT_CLASS::GetTypeID()
#define CREATE_COMPONENT_TYPE_ID_LIST(...) { FOR_EACH(COMPONENT_GET_TYPE_ID, __VA_ARGS__) }

namespace RECS {
	class ISystem
	{
	private:
		std::vector<IComponent*> m_targets;
	public:
		void GetTargets() 
		{
			
		}
	};
}

#endif // !SYSTEMS_H
